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

void RendererSystem::SetupRenderer( const Projection& Proj, float Scale )
{
    Viewport const& Vp = Proj.GetViewport();
    glViewport( Vp.X, Vp.Y, Vp.Width * Scale, Vp.Height * Scale );

    mShaderManager.UploadGlobalData( GlobalShaderData::WorldProjection, mWorldProjector.GetMatrix() );
    mShaderManager.UploadGlobalData( GlobalShaderData::WorldCamera, mCamera.GetView() );
    mShaderManager.UploadGlobalData( GlobalShaderData::InverseProjection, glm::inverse( mCamera.GetView() ) * glm::inverse( mWorldProjector.GetMatrix() ) );
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
Scene::Actors_t getLights()
{
    static int32_t id( AutoId( "lights" ) );
    static Scene& scen( Scene::Get() );
    return scen.GetActorsFromMap( id );
}
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
    SetupRenderer( mWorldProjector );
    // render world
    render::RenderTarget& rt( render::RenderTarget::Get() );
    // allocate render target ids
    static uint32_t const world = rt.GetFreeId();
    static uint32_t const shadowOutline = rt.GetFreeId();
    static uint32_t const shadowDedicatedOutline = rt.GetFreeId();
    static uint32_t const shadowUnwrap = rt.GetFreeId();
    static uint32_t const shadowDedicatedUnwrap = rt.GetFreeId();
    static uint32_t const shadows = rt.GetFreeId();
    static uint32_t const lightsLayer = rt.GetFreeId();
    static uint32_t const lightsDedicated = rt.GetFreeId();
    static uint32_t const worldBumped = rt.GetFreeId();
    static uint32_t const worldEffects = rt.GetFreeId();
    static uint32_t const worldPostProcess = rt.GetFreeId();
    static uint32_t const worldDedicatedPostProcess = rt.GetFreeId();

    RenderTargetProps worldProps( mWorldProjector.GetViewport().Size(), {GL_RGBA, GL_RGB } );
    rt.SetTargetTexture( world, worldProps);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    Scene& Scen( Scene::Get() );
    mPerfTimer.Log( "pre prepare" );
    mActorRenderer.Prepare( Scen, mCamera, DeltaTime );
    mPerfTimer.Log( "post prepare" );
    mActorRenderer.Draw( &selectBloodReceivers );
    mDecalEngine.Draw();

    static bool const castShadows = Settings::Get().GetInt( "graphics.cast_shadows", 1 );
    mPerfTimer.Log( "pre shadow" );
    std::set<int32_t> shadowLevels, postProcessorIds;
    getActiveActorProps( shadowLevels, postProcessorIds );
    static int32_t shid( AutoId( "shadows" ) );
    static int32_t sh2id( AutoId( "shadows2" ) );
    static int32_t lightsid( AutoId( "lights" ) );
    static int32_t unwrapid( AutoId( "shadow_unwrap" ) );
    static int32_t solidid( AutoId( "world_solid_objects" ) );
    static int32_t lightmap( AutoId( "lightmap" ) );
    auto const& lights = getLights();
    if( castShadows != 0 )
    {
        static float const shadowmult = Settings::Get().GetFloat( "graphics.shadow_scale", 0.3 );
        for( auto shadowLevel : shadowLevels )
        {
            bool topmost = shadowLevel == std::numeric_limits<int32_t>::max();
            uint32_t outline = topmost ? shadowOutline : shadowDedicatedOutline;
            uint32_t unwrap = topmost ? shadowUnwrap : shadowDedicatedUnwrap;
            uint32_t lightrl = topmost ? lightsLayer : lightsDedicated;

            rt.SetTargetTexture( outline, RenderTargetProps( mWorldProjector.GetViewport().Size() * shadowmult, { GL_RGBA4 } ) );
            SetupRenderer( mWorldProjector, shadowmult );
            mActorRenderer.Draw( std::bind( &selectShadowCasters, std::placeholders::_1, shadowLevel) );

            rt.SelectTargetTexture( world );
            SetupRenderer( mWorldProjector );
            mActorRenderer.Draw( std::bind( &selectShadowReceivers, std::placeholders::_1, shadowLevel ) );

            rt.SetTargetTexture( lightrl, RenderTargetProps( mWorldProjector.GetViewport().Size() * shadowmult, { GL_RGBA } ) );
            glBlendEquation( GL_MAX );
            for( auto light : lights )
            {
                double radius = light->Get<ILightComponent>()->GetRadius();
                glm::vec2 lightSize( radius, radius );  // not done yet
                auto positionC = light->Get<IPositionComponent>();
                glm::vec4 pos( positionC->GetX(), positionC->GetY(), 1, 1 );
                auto lightPos4 =  mWorldProjector.GetMatrix() * mCamera.GetView() * pos;
                // create camera with max light range range, pos center
                // use that cam + world to render outline to small shadow map
                // use small shadow map to create 1d map
                // use that map + pos to render shadow layer
                SetupRenderer( mWorldProjector, shadowmult );   // needed for resolution
                SetupIdentity();

                glm::vec2 shadowsize( mWorldProjector.GetViewport().Size() * shadowmult );
                glm::vec2 uwsize( shadowsize.x, 1 );
                glm::vec2 lightPos = glm::vec2( lightPos4.x + 1, lightPos4.y + 1 ) / 2.0;
                // lightpos: 0..1 ^2 ( vagy screenen kivul )
                rt.SetTargetTexture( unwrap, RenderTargetProps( uwsize, { GL_RGBA } ) );
                mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( outline ), unwrapid,
                    [&](ShaderManager& ShaderMgr)->void{
                        ShaderMgr.UploadData( "resolution", shadowsize );
                        ShaderMgr.UploadData( "lightPosition", lightPos );
                        ShaderMgr.UploadData( "lightSize", lightSize );
                    } );

                rt.SelectTargetTexture( lightrl );
                mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( unwrap ), lightsid,
                    [&](ShaderManager& ShaderMgr)->void{
                        ShaderMgr.UploadData( "resolution", shadowsize );
                        ShaderMgr.UploadData( "lightPosition", lightPos );
                        ShaderMgr.UploadData( "lightSize", lightSize );
                    } );
            }
            glBlendEquation( GL_FUNC_ADD );

            rt.SelectTargetTexture( world );
            SetupRenderer( mWorldProjector ); // needed for resolution
            // using a small(ish) shadow mult with linear texture mag filter, we can simply render the shadow layer instead of using a more expensive blur filter ( and that even a few times )
            SetupIdentity();
            // !---- lights/shadows
            mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( lightrl ), lightmap ); // , mWorldProjector.GetViewport().Size() * shadowmult );
        }
    }
    else
    {
        mActorRenderer.Draw( &selectNonBloodReceivers );
    }
    mPerfTimer.Log( "post shadow" );

    // render the world to the worldBumped texture using the bump mapping shader
    SetupRenderer( mWorldProjector );
    rt.SetTargetTexture( worldBumped, RenderTargetProps( mWorldProjector.GetViewport().Size() ) );
    SetupIdentity();
    glBlendFunc( GL_ONE, GL_ONE );
    glBlendEquation( GL_MAX );
    static int32_t bumpid = AutoId( "bump_map_mp" );
    glDepthFunc( GL_LEQUAL );
    for( auto light : lights )
    {
        auto positionC = light->Get<IPositionComponent>();
        glm::vec4 pos( positionC->GetX(), positionC->GetY(), 1, 1 );
        auto lightPos4 =  mWorldProjector.GetMatrix() * mCamera.GetView() * pos;
        glm::vec2 lightPos = glm::vec2( lightPos4.x + 1, lightPos4.y + 1 ) / 2.0;

        mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( world ), bumpid,
            [&](ShaderManager& ShaderMgr)->void{
                ShaderMgr.UploadData( "secondaryTexture", GLuint( 2 ) );
                ShaderMgr.UploadData( "resolution", mWorldProjector.GetViewport().Size() );
                ShaderMgr.UploadData( "lightPosition", lightPos );
                glActiveTexture( GL_TEXTURE0 + 2 );
                glBindTexture( GL_TEXTURE_2D, rt.GetTextureId( world, 1 ) );
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
            SetupRenderer( mWorldProjector );
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
        default:
            mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( worldEffects ), solidid );
    }

    SetupRenderer( mWorldProjector );
    render::ParticleEngine::Get().Draw();

    SetupRenderer( mUiProjector );
    mUiRenderer.Draw( mUi.GetRoot(), mUiProjector.GetMatrix() );
    mNameRenderer.Draw( mTextSceneRenderer );
    mPathBoxRenderer.Draw( mTextSceneRenderer );
    mHealthBarRenderer.Draw();
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

