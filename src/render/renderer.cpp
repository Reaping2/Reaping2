#include "i_render.h"
#include "renderer.h"
#include "ui/i_ui.h"
#include "font.h"
#include "particle_engine.h"
#include "render_target.h"
#include "platform/settings.h"
#include "sprite_phase_cache.h"
#include "engine/engine.h"
#include "engine/activity_system.h"
#include "light_system.h"
#include "core/i_light_component.h"
#include "core/i_position_component.h"
#include <boost/assign.hpp>

namespace engine {
namespace {
double GetCamSize( Camera* cam )
{
    static float const activityMult = Settings::Get().GetFloat( "activity.camera_multiplier", 0.5 );
    auto const& view = cam->VisibleRegion();
    return std::max( view.z - view.x, view.w - view.y ) * activityMult;
}
enum ShownLayer
{
    Normal,
    SpriteCache,
    BumpMap,
    PostprocessMask,
    ShadowUnwrap,
    Shadows,
    Lights,
    TopLights,
    AllLights,
};
ShownLayer shownLayer()
{
    static std::string const layer = Settings::Get().GetStr( "graphics.shown_layer", "normal" );
    static std::map<std::string,ShownLayer> const layers = {
        { "normal", Normal },
        { "sprite_cache", SpriteCache },
        { "bump_map", BumpMap },
        { "postprocess_mask", PostprocessMask },
        { "shadow_unwrap", ShadowUnwrap },
        { "shadows", Shadows },
        { "lights", Lights },
        { "top_lights", TopLights },
        { "all_lights", AllLights },
    };
    auto i = layers.find( layer );
    return i == layers.end() ? Normal : i->second;
}
}
RendererSystem::RendererSystem()
    : mWorldProjector( -1000.0f, 1000.0f )
    , mUiProjector( 100.0f, 0.0f, Projection::VM_Fixed )
    , mCamera( mWorldProjector )
    , mUi( Ui::Get() )
    , mDecalEngine( DecalEngine::Get() )
    , mShaderManager( ShaderManager::Get() )
    , mMouseRawPos( 0 )
    , mMouseWorldPos( 0 )
{
    Font::Get();
    mMouseMoveId = EventServer<ScreenMouseMoveEvent>::Get().Subscribe( boost::bind( &RendererSystem::OnMouseMoveEvent, this, _1 ) );
    mMousePressId = EventServer<ScreenMousePressEvent>::Get().Subscribe( boost::bind( &RendererSystem::OnMousePressEvent, this, _1 ) );
    mMouseReleaseId = EventServer<ScreenMouseReleaseEvent>::Get().Subscribe( boost::bind( &RendererSystem::OnMouseReleaseEvent, this, _1 ) );
    core::ActivityTraits::SetActorScaleFunc( std::bind( &RenderableRepo::GetMaxScale, &RenderableRepo::Get(), std::placeholders::_1 ) );
    core::ActivityTraits::SetActiveSizeFunc( std::bind( &GetCamSize, &mCamera ) );
    Init();
}

RendererSystem::~RendererSystem()
{

}

void RendererSystem::SetupIdentity()
{
    static glm::mat4 const id(1.0);
    mShaderManager.UploadGlobalData( GlobalShaderData::WorldProjection, id );
    mShaderManager.UploadGlobalData( GlobalShaderData::WorldCamera, id );
// NOTE: this function is called before world rendering
// and the "original" inverse matrix is required there, not the identity-inverse
// to get the raw world coords from frag coords
//    mShaderManager.UploadGlobalData( GlobalShaderData::InverseProjection, id );
}

void RendererSystem::SetupRenderer( const Camera& cam, float Scale )
{
    auto const& proj = cam.GetProjection();
    Viewport const& Vp = proj.GetViewport();
    glViewport( Vp.X, Vp.Y, Vp.Width * Scale, Vp.Height * Scale );

    mShaderManager.UploadGlobalData( GlobalShaderData::WorldProjection, proj.GetMatrix() );
    mShaderManager.UploadGlobalData( GlobalShaderData::WorldCamera, cam.GetView() );
    mShaderManager.UploadGlobalData( GlobalShaderData::InverseProjection, glm::inverse( cam.GetView() ) * glm::inverse( proj.GetMatrix() ) );
    mShaderManager.UploadGlobalData( GlobalShaderData::Resolution, glm::vec2( Vp.Width * Scale, Vp.Height * Scale ) );
}

void RendererSystem::OnMouseMoveEvent( const ScreenMouseMoveEvent& Event )
{
    glm::vec3 EvtPos( Event.Pos.x, Event.Pos.y, 0 );
    mMouseRawPos = EvtPos;
    glm::vec3 UiEvtPos = mUiProjector.Unproject( EvtPos );
    UiMouseMoveEvent UiEvt( glm::vec2( UiEvtPos.x, UiEvtPos.y ) );
    if( EventServer<UiMouseMoveEvent>::Get().SendEvent( UiEvt ) )
    {
        return;
    }
}

void RendererSystem::OnMousePressEvent( const ScreenMousePressEvent& Event )
{
    glm::vec3 EvtPos( Event.Pos.x, Event.Pos.y, 0 );
    glm::vec3 UiEvtPos = mUiProjector.Unproject( EvtPos );
    UiMousePressEvent UiEvt( glm::vec2( UiEvtPos.x, UiEvtPos.y ), Event.Button );
    if( EventServer<UiMousePressEvent>::Get().SendEvent( UiEvt ) )
    {
        return;
    }

    glm::vec3 WorldEvtPos( mCamera.GetInverseView()*glm::vec4( mWorldProjector.Unproject( EvtPos ), 1.0 ) );

    WorldMousePressEvent WorldEvt( glm::vec2( WorldEvtPos.x, WorldEvtPos.y ), Event.Button );
    EventServer<WorldMousePressEvent>::Get().SendEvent( WorldEvt );
}

void RendererSystem::OnMouseReleaseEvent( const ScreenMouseReleaseEvent& Event )
{
    glm::vec3 EvtPos( Event.Pos.x, Event.Pos.y, 0 );
    glm::vec3 UiEvtPos = mUiProjector.Unproject( EvtPos );
    UiMouseReleaseEvent UiEvt( glm::vec2( UiEvtPos.x, UiEvtPos.y ), Event.Button );
    if( EventServer<UiMouseReleaseEvent>::Get().SendEvent( UiEvt ) )
    {
        return;
    }

    glm::vec3 WorldEvtPos( mCamera.GetInverseView()*glm::vec4( mWorldProjector.Unproject( EvtPos ), 1.0 ) );
    WorldMouseReleaseEvent WorldEvt( glm::vec2( WorldEvtPos.x, WorldEvtPos.y ), Event.Button );
    EventServer<WorldMouseReleaseEvent>::Get().SendEvent( WorldEvt );
}

void RendererSystem::Init()
{
    glEnable( GL_TEXTURE_2D );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_BLEND );
    Scene::Get().AddValidator( AutoId("lights"),
        []( Actor const& actor )->bool {
            return actor.Get<ILightComponent>().IsValid() && actor.Get<IPositionComponent>().IsValid();
        }
        );
}

namespace {
std::set<int32_t> blacklistedPostprocessors()
{
    static bool inited = false;
    static std::set<int32_t> rv;
    if( inited )
    {
        return rv;
    }
    inited = true;
    Json::Value bl = Settings::Get().Resolve( "graphics.pp_blacklist" );
    if( !bl.isArray() )
    {
        return rv;
    }
    for( auto it : bl )
    {
        rv.insert( AutoId( it.asString() ) );
    }
    return rv;
}
void getActiveActorProps( std::set<int32_t>& shadowLevels, std::set<int32_t>& postprocessorIds )
{
    static auto activityS = engine::Engine::Get().GetSystem<engine::ActivitySystem>();
    auto const& Lst = activityS->GetActiveActors();
    std::set<int32_t> pps;
    for( auto i = Lst.begin(), e = Lst.end(); i != e; ++i )
    {
        const Actor& Object = **i;
        Opt<IRenderableComponent> renderableC( Object.Get<IRenderableComponent>() );
        if( !renderableC.IsValid() )
        {
            continue;
        }
        shadowLevels.insert( renderableC->GetCastShadow() );
        shadowLevels.insert( renderableC->GetReceiveShadow() );
        auto const& procs = renderableC->GetPostProcessIds();
        pps.insert( procs.begin(), procs.end() );
    }
    shadowLevels.erase( 0 );
    static auto const bls = blacklistedPostprocessors();
    std::set_difference( pps.begin(), pps.end(), bls.begin(), bls.end(), std::inserter( postprocessorIds, postprocessorIds.end() ) );
}
bool selectBloodReceivers( IRenderableComponent const& renderableC )
{
    return renderableC.GetReceiveBlood() != 0;
}
bool selectNonBloodReceivers( IRenderableComponent const& renderableC )
{
    return renderableC.GetReceiveBlood() == 0;
}
bool selectShadowReceivers( IRenderableComponent const& renderableC, int32_t shadowLevel )
{
    return renderableC.GetReceiveBlood() == 0 && renderableC.GetReceiveShadow() == shadowLevel;
}

bool selectShadowCasters( IRenderableComponent const& renderableC, int32_t shadowLevel )
{
    return renderableC.GetReceiveBlood() == 0 && renderableC.GetCastShadow() == shadowLevel;
}

bool selectShadowCastersExcept( IRenderableComponent const& renderableC, int32_t shadowLevel, IRenderableComponent const* except )
{
    return &renderableC != except && renderableC.GetReceiveBlood() == 0 && renderableC.GetCastShadow() == shadowLevel;
}
struct LightDesc
{
    Opt<ILightComponent> lightC;
    Opt<IRenderableComponent> renderableC;
    glm::vec2 center;
    GLfloat orientation;
};
std::vector<LightDesc> getLights()
{
    static auto lightS = engine::Engine::Get().GetSystem<render::LightSystem>();
    std::vector<LightDesc> lds;
    for( auto light : lightS->GetActiveLights() )
    {
        auto positionC = light->Get<IPositionComponent>();
        glm::vec2 center( std::numeric_limits<double>::max(), std::numeric_limits<double>::max() );
        GLfloat heading = 0.0;
        if( positionC.IsValid() )
        {
            center = glm::vec2( positionC->GetX(), positionC->GetY() );
            heading = positionC->GetOrientation();
        }
        lds.push_back(
            std::move( LightDesc{ light->Get<ILightComponent>(),
            light->Get<IRenderableComponent>(),
            center,
            heading } )
            );
    }
    lds.push_back( 
        std::move( LightDesc{ Opt<ILightComponent>(),
        Opt<IRenderableComponent>(),
        glm::vec2( std::numeric_limits<double>::max(), std::numeric_limits<double>::max() ),
        0.0 } )
        );
    return lds;
}
}

void RendererSystem::Update( double DeltaTime )
{
    using render::RenderTargetProps;
    perf::Timer_t method;
    method.Log( "start render" );
    static render::SpritePhaseCache& cache( render::SpritePhaseCache::Get() );
    render::ParticleEngine::Get().Update( DeltaTime );
    SendWorldMouseMoveEvent();

    mCamera.Update();
    SetupRenderer( mCamera );
    // render world
    render::RenderTarget& rt( render::RenderTarget::Get() );
    // allocate render target ids
    static uint32_t const world = rt.GetFreeId();
    static uint32_t const shadowOutline = rt.GetFreeId();
    static uint32_t const shadowDedicatedOutline = rt.GetFreeId();
    static uint32_t const shadowUnwrap = rt.GetFreeId();
    static uint32_t const shadowDedicatedUnwrap = rt.GetFreeId();
    static uint32_t const lightsLayer = rt.GetFreeId();
    static uint32_t const lightsDedicated = rt.GetFreeId();
    static uint32_t const worldBumped = rt.GetFreeId();
    static uint32_t const worldEffects = rt.GetFreeId();
    static uint32_t const worldPostProcess = rt.GetFreeId();
    static uint32_t const worldDedicatedPostProcess = rt.GetFreeId();
    static uint32_t const sunDedicatedOutline = rt.GetFreeId();
    static uint32_t const cumulativeLight = rt.GetFreeId();
    static uint32_t const topcasters = rt.GetFreeId();

    static auto lightS = engine::Engine::Get().GetSystem<render::LightSystem>();
    auto const& lights = getLights();

    std::vector<Camera const*> cameras;
    Projection lightCamProjection( -500, 500 );
    float distanceMult = mWorldProjector.GetVisibleRegion().y * 1.0f / lightCamProjection.GetVisibleRegion().y;
    std::vector<std::unique_ptr<Camera> > tempCameras;
    cameras.push_back( &mCamera );
    // add cameras for lights
    for( auto const& light : lights )
    {
        std::unique_ptr<Camera> cam( new Camera( lightCamProjection ) );
        cam->SetCenter( light.center );
        cameras.push_back( cam.get() );
        tempCameras.push_back( std::move( cam ) );
    }

    RenderTargetProps worldProps( mWorldProjector.GetViewport().Size(), {GL_RGBA, GL_RGB } );
    rt.SetTargetTexture( world, worldProps);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    Scene& Scen( Scene::Get() );
    mPerfTimer.Log( "pre prepare" );
    mActorRenderer.Prepare( Scen, cameras, DeltaTime );
    mPerfTimer.Log( "post prepare" );
    mActorRenderer.Draw( &selectBloodReceivers );
    mDecalEngine.Draw();

    static bool const castShadows = Settings::Get().GetInt( "graphics.cast_shadows", 1 );
    mPerfTimer.Log( "pre shadow" );
    std::set<int32_t> shadowLevels, postProcessorIds;
    getActiveActorProps( shadowLevels, postProcessorIds );
    static int32_t lightsid( AutoId( "lights" ) );
    static int32_t unwrapid( AutoId( "shadow_unwrap" ) );
    static int32_t solidid( AutoId( "world_solid_objects" ) );
    static int32_t sunlight( AutoId( "sunlight" ) );
    static int32_t topcastersid( AutoId( "topcasters" ) );
    static int32_t lightmap( AutoId( "lightmap" ) );
    static int32_t mergelights( AutoId( "mergelights" ) );
    static float const shadowmult = Settings::Get().GetFloat( "graphics.shadow_scale", 0.3 );
    rt.SetTargetTexture( cumulativeLight, RenderTargetProps( mWorldProjector.GetViewport().Size() * shadowmult, { GL_RGBA } ) );
    float maxShadow = lightS->GetMaxShadow();
    glm::vec2 lightVec = lightS->GetShadowVector();
    glm::vec4 ambientLight = lightS->GetAmbientLight();
    int numShadowSteps = ceil( std::max( std::abs( lightVec.x ), std::abs( lightVec.y ) ) / 10.0 );
    LL() << maxShadow << " (" << lightVec.x
        << " " << lightVec.y
        << ") ("
        << ambientLight.x << " "
        << ambientLight.y << " "
        << ambientLight.z << " "
        << ambientLight.w
        << ") "
        << numShadowSteps;
    glClearColor( 1,1,1,1 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    if( castShadows != 0 )
    {
        for( auto shadowLevel : shadowLevels )
        {
            bool topmost = shadowLevel == std::numeric_limits<int32_t>::max();
            uint32_t outline = topmost ? shadowOutline : shadowDedicatedOutline;
            uint32_t unwrap = topmost ? shadowUnwrap : shadowDedicatedUnwrap;
            uint32_t lightrl = topmost ? lightsLayer : lightsDedicated;
            uint32_t sunline = topmost ? shadowOutline : sunDedicatedOutline;

            glBlendEquation( GL_FUNC_ADD );
            rt.SelectTargetTexture( world );
            SetupRenderer( mCamera );
            mActorRenderer.Draw( std::bind( &selectShadowReceivers, std::placeholders::_1, shadowLevel ) );

            rt.SetTargetTexture( lightrl, RenderTargetProps( mWorldProjector.GetViewport().Size() * shadowmult, { GL_RGBA, GL_RGBA } ) );
            rt.SelectTargetTexture( lightrl, true );

            // lightrl collects the lights, we have to init it to 0 - complete darkness
            // we use GL_MAX when drawing to it
            glClearColor( 0, 0, 0, 1 );
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
            rt.SelectTargetTexture( lightrl );
            glBlendEquation( GL_MAX );

            // -- direct sunlight to outline
            // render the normal shadowcasters with a "tail" ( lightVec )
            rt.SetTargetTexture( sunline, RenderTargetProps( mCamera.GetProjection().GetViewport().Size() * shadowmult, { GL_RGBA4, GL_RGB4 } ) );
            SetupRenderer( mCamera, shadowmult );
            mActorRenderer.Draw(
                std::bind( &selectShadowCasters, std::placeholders::_1, shadowLevel),
                [&](ShaderManager& ShaderMgr)->void{
                    ShaderMgr.UploadData( "resolution", mCamera.GetProjection().GetViewport().Size() * shadowmult );
                    ShaderMgr.UploadData( "lightVec", lightVec );
                },
                numShadowSteps
            );
            // fill lights except outline
            // use the previously rendered sun outline to draw into the lightrl map
            // lightrl: r/g/b is the alpha from sunline
            // as GL_MAX uses rgb, no alpha
            SetupIdentity();
            rt.SelectTargetTexture( lightrl );
            mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( sunline ), sunlight,
                [&](ShaderManager& ShaderMgr)->void{
                    ShaderMgr.UploadData( "resolution", mCamera.GetProjection().GetViewport().Size() * shadowmult );
                    ShaderMgr.UploadData( "maxShadow", maxShadow );
                    ShaderMgr.UploadData( "ambient", ambientLight );
                } );
            // direct sunlight end

            auto lightCamIt = tempCameras.begin();
            for( auto const& light : lights )
            {
                auto const& lightCam = **lightCamIt++;
                GLfloat lightSize = light.lightC.IsValid() ? light.lightC->GetRadius() : -1.0;
                glm::vec4 pos( light.center.x, light.center.y, 1, 1 );
                GLfloat ori = light.orientation; // radians
                GLfloat aperture = ( light.lightC.IsValid() ? light.lightC->GetAperture() : 0 ) * 3.141592654 / 180.0;
                GLfloat fsaperture = ( light.lightC.IsValid() ? light.lightC->GetFullStrengthAperture() : 0 ) * 3.141592654 / 180.0;
                // lightPos4 is in player view space
                auto lightPos4 =  mCamera.GetProjection().GetMatrix() * mCamera.GetView() * pos;
                // create camera with max light range range, pos center
                // use that cam + world to render outline to small shadow map
                // use small shadow map to create 1d map
                // use that map + pos to render shadow layer
                rt.SetTargetTexture( outline, RenderTargetProps( lightCam.GetProjection().GetViewport().Size() * shadowmult, { GL_RGBA4 } ) );
                SetupRenderer( lightCam, shadowmult );
                mActorRenderer.Draw( std::bind( &selectShadowCastersExcept,
                            std::placeholders::_1,
                            shadowLevel,
                            light.renderableC.Get() ) );

                SetupIdentity();

                glm::vec2 shadowsize( lightCam.GetProjection().GetViewport().Size() * shadowmult );
                glm::vec2 uwsize( shadowsize.x, 1 );
                glm::vec2 lightPos = glm::vec2( lightPos4.x + 1, lightPos4.y + 1 ) / 2.0;
                glm::vec2 lightPosInShadowTex( 0.5, 0.5 );
                // lightpos: 0..1 ^2 ( vagy screenen kivul )
                rt.SetTargetTexture( unwrap, RenderTargetProps( uwsize, { GL_RGBA } ) );
                mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( outline ), unwrapid,
                    [&](ShaderManager& ShaderMgr)->void{
                        ShaderMgr.UploadData( "resolution", shadowsize );
                        ShaderMgr.UploadData( "lightPosition", lightPosInShadowTex );
                        ShaderMgr.UploadData( "lightSize", lightSize );
                    } );

                rt.SelectTargetTexture( lightrl );
                mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( unwrap ), lightsid,
                    [&](ShaderManager& ShaderMgr)->void{
                        ShaderMgr.UploadData( "resolution", mCamera.GetProjection().GetViewport().Size() * shadowmult );
                        ShaderMgr.UploadData( "lightPosition", lightPos );
                        ShaderMgr.UploadData( "lightSize", lightSize );
                        ShaderMgr.UploadData( "distanceMult", distanceMult );
                        ShaderMgr.UploadData( "maxShadow", maxShadow );
                        ShaderMgr.UploadData( "heading", ori );
                        ShaderMgr.UploadData( "aperture", aperture );
                        ShaderMgr.UploadData( "fsaperture", fsaperture );
                    } );
            }

            // !---- cumulative lights for normal maps
            glBlendEquation( GL_MIN );
            rt.SelectTargetTexture( cumulativeLight );
            mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( lightrl, 1 ), mergelights,
                     [&](ShaderManager& ShaderMgr)->void{
                        ShaderMgr.UploadData( "resolution", mCamera.GetProjection().GetViewport().Size() );
                        ShaderMgr.UploadData( "maxShadow", maxShadow );
                    } );
            glBlendEquation( GL_FUNC_ADD );
            SetupRenderer( mCamera, shadowmult );
            // remove self-cast shadows from the normal map mask
            // note: we use the alpha channel only from the cumul. lights map, so we can simply use the default draw shader. yay.
            mActorRenderer.Draw( std::bind( &selectShadowCasters, std::placeholders::_1, shadowLevel) );

            glBlendEquation( GL_FUNC_ADD );
            if( topmost )  // on topmost level, only the shadow receivers should be visibly illuminated, not the lower shadow receiver layers
            {
                rt.SetTargetTexture( topcasters, RenderTargetProps( mCamera.GetProjection().GetViewport().Size() * shadowmult, { GL_RGBA4, GL_RGB4 } ) );
                mActorRenderer.Draw( std::bind( &selectShadowReceivers, std::placeholders::_1, shadowLevel ) );
                glBlendFuncSeparate( GL_ZERO, GL_ONE, GL_DST_ALPHA, GL_ZERO );
                rt.SelectTargetTexture( lightrl );
                SetupIdentity();
                // !---- lights/shadows
                mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( topcasters ), topcastersid );
            }

            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            rt.SelectTargetTexture( world );
            SetupRenderer( mCamera );
            // using a small(ish) shadow mult with linear texture mag filter, we can simply render the shadow layer instead of using a more expensive blur filter ( and that even a few times )
            SetupIdentity();
            // !---- lights/shadows
            mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( lightrl ), lightmap );
        }
    }
    else
    {
        mActorRenderer.Draw( &selectNonBloodReceivers );
    }
    mPerfTimer.Log( "post shadow" );

    // render the world to the worldBumped texture using the bump mapping shader
    SetupRenderer( mCamera );
    rt.SetTargetTexture( worldBumped, RenderTargetProps( mWorldProjector.GetViewport().Size() ) );
    SetupIdentity();
    glBlendFunc( GL_ONE, GL_ONE );
    glBlendEquation( GL_MAX );
    static int32_t bumpid = AutoId( "bump_map_mp" );
    glDepthFunc( GL_LEQUAL );
    for( auto const& light : lights )
    {
        auto lightPos4 =  mWorldProjector.GetMatrix() * mCamera.GetView()
            * glm::vec4( light.center.x, light.center.y, 1, 1 );
        glm::vec2 lightPos = glm::vec2( lightPos4.x + 1, lightPos4.y + 1 ) / 2.0;
        GLfloat lightSize = light.lightC.IsValid() ? light.lightC->GetRadius() : -1.0;

        mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( world ), bumpid,
            [&](ShaderManager& ShaderMgr)->void{
                ShaderMgr.UploadData( "secondaryTexture", GLuint( 2 ) );
                ShaderMgr.UploadData( "lightTexture", GLuint( 3 ) );
                ShaderMgr.UploadData( "resolution", mWorldProjector.GetViewport().Size() );
                ShaderMgr.UploadData( "lightPosition", lightPos );
                ShaderMgr.UploadData( "lightSize", lightSize );
                glActiveTexture( GL_TEXTURE0 + 2 );
                glBindTexture( GL_TEXTURE_2D, rt.GetTextureId( world, 1 ) );
                glActiveTexture( GL_TEXTURE0 + 3 );
                glBindTexture( GL_TEXTURE_2D, rt.GetTextureId( cumulativeLight ) );
            } );
    }
    glBlendFunc( GL_ONE, GL_ONE );
    glBlendEquation( GL_FUNC_ADD );
    glDepthFunc( GL_LESS );

    rt.SetTargetTexture( worldEffects, RenderTargetProps( mWorldProjector.GetViewport().Size() ) );
    mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( worldBumped ), solidid );

    static bool const enablePostprocessing = Settings::Get().GetBool( "graphics.postprocess", true );
    if( enablePostprocessing )
    {
        RenderTargetProps worldPPProps( mWorldProjector.GetViewport().Size(), { GL_RED } );
        // render the per-actor effects
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        for( auto id : postProcessorIds )
        {
            // TODO: downscale
            static int32_t debuggedPP = AutoId( Settings::Get().GetStr( "graphics.shown_layer_pp_id", "" ) );
            uint32_t pp = id == debuggedPP ? worldDedicatedPostProcess : worldPostProcess;
            rt.SetTargetTexture( pp, worldPPProps );
            SetupRenderer( mCamera );
            mActorRenderer.Draw( id );

            GLuint srcTexture = rt.GetTextureId( worldBumped );
            GLuint maskTexture = rt.GetTextureId( pp );
            rt.SelectTargetTexture( worldEffects );
            SetupIdentity();
            mWorldRenderer.Draw( DeltaTime, srcTexture, id, maskTexture );
        }
    }
    // set painting to screen
    rt.SetTargetScreen();
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    SetupIdentity();

    // paint the previous textures to screen with custom additional effects
    // actually we could skip this by painting directly to screen in prev. step
    // but we can possibly upscale here for sweet sweet fps

    static auto const layer = shownLayer();
    switch( layer )
    {
        case PostprocessMask:
            mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( worldDedicatedPostProcess ), solidid );
            break;
        case SpriteCache:
            mWorldRenderer.Draw( DeltaTime, cache.mTargetTexId, solidid );
            break;
        case BumpMap:
            mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( world, 1 ), solidid );
            break;
        case ShadowUnwrap:
            mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( shadowDedicatedUnwrap ), solidid );
            break;
        case Shadows:
            mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( shadowDedicatedOutline ), solidid );
            break;
        case Lights:
            mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( lightsDedicated ), solidid );
            break;
        case TopLights:
            mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( lightsLayer ), solidid );
            break;
        case AllLights:
            mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( cumulativeLight ), solidid );
            break;
        default:
            mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( worldEffects ), solidid );
    }

    SetupRenderer( mCamera );
    render::ParticleEngine::Get().Draw();

    Viewport const& Vp = mUiProjector.GetViewport();
    glViewport( Vp.X, Vp.Y, Vp.Width, Vp.Height );
    mShaderManager.UploadGlobalData( GlobalShaderData::Resolution, glm::vec2( Vp.Width, Vp.Height ) );

    static bool const showNames = Settings::Get().GetBool( "graphics.show_names", true );
    static bool const showHealthbars = Settings::Get().GetBool( "graphics.show_healthbars", true );

    mUiRenderer.Draw( mUi.GetRoot(), mUiProjector.GetMatrix() );
    if( showNames )
    {
        mNameRenderer.Draw( mTextSceneRenderer );
    }
    mPathBoxRenderer.Draw( mTextSceneRenderer );
    if( showHealthbars )
    {
        mHealthBarRenderer.Draw();
    }
    mMouseRenderer.Draw( mTextSceneRenderer );
    mTextSceneRenderer.Draw();
    method.Log( "end draw" );
    cache.ProcessPending();
    method.Log( "end process pending" );
    method.Log( "end render" );
}

void RendererSystem::SendWorldMouseMoveEvent()
{
    glm::vec3 newMouseWorldPos = glm::vec3( mCamera.GetInverseView() * glm::vec4( mWorldProjector.Unproject( mMouseRawPos ), 1.0 ) );
    if ( newMouseWorldPos != mMouseWorldPos )
    {
        mMouseWorldPos = newMouseWorldPos;
        WorldMouseMoveEvent WorldEvt( glm::vec2( mMouseWorldPos.x, mMouseWorldPos.y ) );
        EventServer<WorldMouseMoveEvent>::Get().SendEvent( WorldEvt );
    }
}

Camera const& RendererSystem::GetCamera() const
{
    return mCamera;
}


} // namespace engine

