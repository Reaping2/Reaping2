#include "i_render.h"
#include "renderer.h"
#include "ui/i_ui.h"
#include "font.h"
#include "particle_engine.h"
#include "render_target.h"
#include "platform/settings.h"
#include "sprite_phase_cache.h"
#include <boost/assign.hpp>

namespace engine {
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
    Init();
}

RendererSystem::~RendererSystem()
{

}

void RendererSystem::SetupRenderer( const Projection& Proj )
{
    Viewport const& Vp = Proj.GetViewport();
    glViewport( Vp.X, Vp.Y, Vp.Width, Vp.Height );

    mShaderManager.UploadGlobalData( GlobalShaderData::WorldProjection, mWorldProjector.GetMatrix() );
    mShaderManager.UploadGlobalData( GlobalShaderData::WorldCamera, mCamera.GetView() );
    //mShaderManager.UploadGlobalData( GlobalShaderData::UiProjection, mUiProjector.GetMatrix() );
}

bool RendererSystem::BeginRender()
{
    return true;
}

bool RendererSystem::EndRender()
{
    return true;
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
}

namespace {
std::set<int32_t> getShadowLevels( Scene const& scene )
{
    std::set<int32_t> rv;
    auto const& Lst = scene.GetActors();
    ActorListFilter<Scene::RenderableActors> wrp( Lst );
    for( auto i = wrp.begin(), e = wrp.end(); i != e; ++i )
    {
        const Actor& Object = **i;
        Opt<IRenderableComponent> renderableC( Object.Get<IRenderableComponent>() );
        rv.insert( renderableC->GetCastShadow() );
        rv.insert( renderableC->GetReceiveShadow() );
    }
    rv.erase( 0 );
    return rv;
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
    perf::Timer_t method;
    method.Log( "start render" );
    static render::SpritePhaseCache& cache( render::SpritePhaseCache::Get() );
    render::ParticleEngine::Get().Update( DeltaTime );
    SendWorldMouseMoveEvent();

    BeginRender();

    mCamera.Update();
    SetupRenderer( mWorldProjector );
    // render world
    render::RenderTarget& rt( render::RenderTarget::Get() );

    // paint solid objects to texture target 1
    // !---- rt 1
    uint32_t world = 1;
    rt.SetTargetTexture( world, mWorldProjector.GetViewport().Size() );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    Scene& Scen( Scene::Get() );
    mPerfTimer.Log( "pre prepare" );
    mActorRenderer.Prepare( Scen, mCamera, DeltaTime );
    mPerfTimer.Log( "post prepare" );
    mActorRenderer.Draw( &selectBloodReceivers );
    mDecalEngine.Draw();

    static bool const castShadows = Settings::Get().GetInt( "graphics.cast_shadows", 1 );
    mPerfTimer.Log( "pre shadow" );
    if( castShadows != 0 )
    {
        uint32_t shadowOutline = 2, shadow_1=4, shadow_2=5;

        auto const shadowLevels = getShadowLevels( Scen );
        for( auto shadowLevel : shadowLevels )
        {
            // !---- rt - shadows outline
            rt.SetTargetTexture( shadowOutline, mWorldProjector.GetViewport().Size() );
            SetupRenderer( mWorldProjector );
            mActorRenderer.Draw( std::bind( &selectShadowCasters, std::placeholders::_1, shadowLevel) );

            mShaderManager.UploadGlobalData( GlobalShaderData::WorldProjection, glm::mat4( 1.0 ) );
            mShaderManager.UploadGlobalData( GlobalShaderData::WorldCamera, glm::mat4( 1.0 )  );


            // !---- rt4 - shadows - with depth
            rt.SetTargetTexture( shadow_1, mWorldProjector.GetViewport().Size() );
            mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( shadowOutline ), "shadows" );
            // !---- rt5 - shadows - vblurred
            rt.SetTargetTexture( shadow_2, mWorldProjector.GetViewport().Size() );
            mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( shadow_1 ), "vblur" );

            rt.SelectTargetTexture( world );
            SetupRenderer( mWorldProjector );
            mActorRenderer.Draw( std::bind( &selectShadowReceivers, std::placeholders::_1, shadowLevel ) );

            mShaderManager.UploadGlobalData( GlobalShaderData::WorldProjection, glm::mat4( 1.0 ) );
            mShaderManager.UploadGlobalData( GlobalShaderData::WorldCamera, glm::mat4( 1.0 )  );

            mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( shadow_2 ), "hblur" );
            mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( shadow_2 ), "hblur" );
        }
    }
    else
    {
        mActorRenderer.Draw( &selectNonBloodReceivers );
    }
    mPerfTimer.Log( "post shadow" );
    // !---- rt16
    // particle blending happens with different blending modes
    // so we can't simply render the particles to their own FBO
    // we render the background with effects, render the particles to a new FBO
    // and at last render the results onto the screen with another layer of effects
    uint32_t worldEffects = 16;
    rt.SetTargetTexture( worldEffects, mWorldProjector.GetViewport().Size() );
    mShaderManager.UploadGlobalData( GlobalShaderData::WorldProjection, glm::mat4( 1.0 ) );
    mShaderManager.UploadGlobalData( GlobalShaderData::WorldCamera, glm::mat4( 1.0 )  );
    glBlendFunc( GL_ONE, GL_ONE );
    mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( world ), "world_solid_objects" );
    SetupRenderer( mWorldProjector );

    // set painting to screen
    rt.SetTargetScreen();
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    mShaderManager.UploadGlobalData( GlobalShaderData::WorldProjection, glm::mat4( 1.0 ) );
    mShaderManager.UploadGlobalData( GlobalShaderData::WorldCamera, glm::mat4( 1.0 )  );

    glClear( GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // paint the previous textures to screen with custom additional effects
    mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( worldEffects ), "world_solid_objects" );
//    mWorldRenderer.Draw( DeltaTime, cache.mTargetTexId, "world_solid_objects" );
    SetupRenderer( mWorldProjector );
    render::ParticleEngine::Get().Draw();

    SetupRenderer( mUiProjector );
    mUiRenderer.Draw( mUi.GetRoot(), mUiProjector.GetMatrix() );
    mNameRenderer.Draw( mTextSceneRenderer );
    mPathBoxRenderer.Draw( mTextSceneRenderer );
    mHealthBarRenderer.Draw();
    mMouseRenderer.Draw( mTextSceneRenderer );
    mTextSceneRenderer.Draw();
    EndRender();
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

