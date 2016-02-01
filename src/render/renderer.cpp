#include "i_render.h"
#include "renderer.h"
#include "ui/i_ui.h"
#include "font.h"
#include "particle_engine.h"
#include "render_target.h"
#include <boost/assign.hpp>

namespace engine {
RendererSystem::RendererSystem()
    : mWorldProjector( -1000.0f, 1000.0f )
    , mUiProjector( 0.0f, 100.0f, Projection::VM_Fixed )
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

void RendererSystem::Update( double DeltaTime )
{
    render::ParticleEngine::Get().Update( DeltaTime );
    SendWorldMouseMoveEvent();

    BeginRender();

    mCamera.Update();

    // render world
    render::RenderTarget& rt( render::RenderTarget::Get() );
    SetupRenderer( mUiProjector );

    // paint solid objects to texture target 1
    rt.SetTargetTexture( 1 );
    glClearColor( 0, 1, 0, 0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    Scene& Scen( Scene::Get() );
    mSceneRenderer.Draw( Scen );
    static std::set<RenderableLayer::Type> const bglayers = boost::assign::list_of( RenderableLayer::Background ).to_container( bglayers );
    static std::set<RenderableLayer::Type> const fglayers;
    mActorRenderer.Draw( Scen, DeltaTime, bglayers, fglayers);
    mDecalEngine.Draw( DecalEngine::GroundParticle );
    mActorRenderer.Draw( Scen, DeltaTime, fglayers, bglayers);

    // particle blending happens with different blending modes
    // so we can't simply render the particles to their own FBO
    // we render the background with effects, render the particles to a new FBO
    // and at last render the results onto the screen with another layer of effects
    rt.SetTargetTexture( 2 );
    mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( 1 ) );
    render::ParticleEngine::Get().Draw();

    // set painting to screen
    rt.SetTargetScreen();
    SetupRenderer( mUiProjector );
    glClear( GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // paint the previous textures to screen with custom additional effects
    mWorldRenderer.Draw( DeltaTime, rt.GetTextureId( 2 ) );

    mUiRenderer.Draw( mUi.GetRoot(), mUiProjector.GetMatrix() );
    mNameRenderer.Draw( mTextSceneRenderer );
    mHealthBarRenderer.Draw();
    mMouseRenderer.Draw( mTextSceneRenderer );
    mTextSceneRenderer.Draw();
    EndRender();
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
