#include "i_render.h"
#include "renderer.h"
#include "ui/i_ui.h"
#include "font.h"
#include "particle_engine.h"
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
    glClear( GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
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
    SetupRenderer( mUiProjector );
    Scene& Scen( Scene::Get() );
    static std::set<RenderableLayer::Type> const bglayers = boost::assign::list_of( RenderableLayer::Background ).to_container( bglayers );
    static std::set<RenderableLayer::Type> const fglayers;
    mActorRenderer.Draw( Scen, DeltaTime, bglayers, fglayers );
    mDecalEngine.Draw();
    mActorRenderer.Draw( Scen, DeltaTime, fglayers, bglayers );
    render::ParticleEngine::Get().Draw();
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
