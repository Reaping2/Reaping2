#include "platform/i_platform.h"
#include "editor_actor_system.h"
#include "ui/ui.h"
#include "editor_grid_system.h"
#include "editor_target_system.h"
#include "editor_brush_system.h"
#include "../../engine/engine.h"
#include "../../input/keyboard.h"
#include "editor_hud_state.h"

namespace map {

EditorActorSystem::EditorActorSystem()
    : mScene( Scene::Get() )
{
}


void EditorActorSystem::Init()
{
    mOnEditorModeChanged = EventServer<map::EditorModeChangedEvent>::Get().Subscribe( boost::bind( &EditorActorSystem::OnEditorModeChanged, this, _1 ) );
    mOnEditorBack = EventServer<map::EditorBackEvent>::Get().Subscribe( boost::bind( &EditorActorSystem::OnEditorBack, this, _1 ) );
}


void EditorActorSystem::Update(double DeltaTime)
{
    if (!mEnabled)
    {
        return;
    }
    static Opt<engine::KeyboardSystem> keyboard = ::engine::Engine::Get().GetSystem<engine::KeyboardSystem>();
    if (keyboard->GetKey( GLFW_KEY_SPACE ).State == KeyState::Typed)
    {
        if (EditorHudState::Get().IsHudShown())
        {
            EnableSubsystems( true );
            EventServer<EditorBackEvent>::Get().SendEvent( EditorBackEvent( true ) );
            EditorTargetSystem::Get()->AddCursor();
        }
        else
        {
            EnableSubsystems( false );
            Ui::Get().Load( "editor_actor_hud" );
            EditorTargetSystem::Get()->RemoveCursor();
            EditorHudState::Get().SetHudShown( true );
        }
    }
}

void EditorActorSystem::OnEditorModeChanged(map::EditorModeChangedEvent const& Evt)
{
    if (Evt.mMode == "actor")
    {
        EnableSubsystems( false );
        ::engine::Engine::Get().SetEnabled<EditorActorSystem>( true );
        Ui::Get().Load( "editor_actor_hud" );
        EditorHudState::Get().SetHudShown( true );
    }
    else
    {
        EnableSubsystems( false );
        ::engine::Engine::Get().SetEnabled<EditorActorSystem>( false );
        EditorTargetSystem::Get()->RemoveCursor();
    }
}

void EditorActorSystem::OnEditorBack( map::EditorBackEvent const& Evt )
{
    if (mEnabled)
    {
        if (!Evt.mBackToBaseHud)
        {
            EnableSubsystems( false );
            Ui::Get().Load( "editor_actor_hud" );
            EditorTargetSystem::Get()->RemoveCursor();
            EditorHudState::Get().SetHudShown( true );
        }
    }
}

void EditorActorSystem::EnableSubsystems( bool enable )
{
    ::engine::Engine::Get().SetEnabled<EditorGridSystem>( enable );
    ::engine::Engine::Get().SetEnabled<EditorTargetSystem>( enable );
    ::engine::Engine::Get().SetEnabled<EditorBrushSystem>( enable );
}

} // namespace map

