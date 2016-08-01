#include "platform/i_platform.h"
#include "property_editor_system.h"
#include "ui/ui.h"
#include "editor_grid_system.h"
#include "editor_target_system.h"
#include "editor_brush_system.h"
#include "../../engine/engine.h"
#include "../../input/keyboard.h"
#include "editor_hud_state.h"
#include "room_start_editor_system.h"
#include "room_end_editor_system.h"
#include "spawn_editor_system.h"

namespace map {

PropertyEditorSystem::PropertyEditorSystem()
    : mScene( Scene::Get() )
{
}


void PropertyEditorSystem::Init()
{
    mOnEditorModeChanged = EventServer<map::EditorModeChangedEvent>::Get().Subscribe( boost::bind( &PropertyEditorSystem::OnEditorModeChanged, this, _1 ) );
    mOnEditorBack = EventServer<map::EditorBackEvent>::Get().Subscribe( boost::bind( &PropertyEditorSystem::OnEditorBack, this, _1 ) );
}


void PropertyEditorSystem::Update(double DeltaTime)
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
            EventServer<EditorBackEvent>::Get().SendEvent( EditorBackEvent( true ) );
        }
        else
        {
            Ui::Get().Load( "editor/room_prop_hud" );
            EditorHudState::Get().SetHudShown( true );
        }
    }
}

void PropertyEditorSystem::OnEditorModeChanged(map::EditorModeChangedEvent const& Evt)
{
    if (Evt.mMode == "room_property")
    {
        ::engine::Engine::Get().SetEnabled<PropertyEditorSystem>( true );
        Ui::Get().Load( "editor/room_prop_hud" );
        EditorHudState::Get().SetHudShown( true );
    }
    else
    {
        ::engine::Engine::Get().SetEnabled<PropertyEditorSystem>( false );
    }
}

void PropertyEditorSystem::OnEditorBack( map::EditorBackEvent const& Evt )
{
    if (mEnabled)
    {
        if (!Evt.mBackToBaseHud)
        {
            Ui::Get().Load( "editor/room_prop_hud" );
            EditorHudState::Get().SetHudShown( true );
        }
//         else
//         {
//             ::engine::Engine::Get().SetEnabled<PropertyEditorSystem>( false );
//         }
    }
}

void PropertyEditorSystem::EnableSubsystems( bool enable )
{
    ::engine::Engine::Get().SetEnabled<RoomStartEditorSystem>( enable );
    ::engine::Engine::Get().SetEnabled<RoomEndEditorSystem>( enable );
    ::engine::Engine::Get().SetEnabled<SpawnEditorSystem>( enable );
}

} // namespace map

