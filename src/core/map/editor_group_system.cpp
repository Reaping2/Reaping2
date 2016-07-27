#include "platform/i_platform.h"
#include "editor_group_system.h"
#include "ui/ui.h"
#include "editor_grid_system.h"
#include "editor_target_system.h"
#include "editor_brush_system.h"
#include "../../engine/engine.h"
#include "../../input/keyboard.h"
#include "editor_hud_state.h"
#include "../actor.h"
#include "../actor_factory.h"
#include "../i_position_component.h"
#include "../i_cell_component.h"
#include "../i_collision_component.h"
#include "input/mouse.h"
#include "group_selected_event.h"
#include "map_element_factory.h"
#include "group_map_element.h"
#include "map_system.h"

namespace map {

EditorGroupSystem::EditorGroupSystem()
    : mScene( Scene::Get() )
    , mEditorGroupModel( "editor_group", &RootModel::Get() )
    , mNewGroupModel( VoidFunc( this, &EditorGroupSystem::OnNewGroup ), "new_group", &mEditorGroupModel )
    , mGroupNamesModel( (ModelValue::get_string_vec_t) RefTo( mGroupNames ), "groups", &mEditorGroupModel )
    , mGroupSelectedModel( StringFunc( this, &EditorGroupSystem::OnGroupSelected ), "group_selected", &mEditorGroupModel )
{
}


void EditorGroupSystem::Init()
{
    mOnEditorModeChanged = EventServer<map::EditorModeChangedEvent>::Get().Subscribe( boost::bind( &EditorGroupSystem::OnEditorModeChanged, this, _1 ) );
    mOnEditorBack = EventServer<map::EditorBackEvent>::Get().Subscribe( boost::bind( &EditorGroupSystem::OnEditorBack, this, _1 ) );
}


void EditorGroupSystem::Update(double DeltaTime)
{
    if (!mEnabled)
    {
        return;
    }
    static Opt<engine::KeyboardSystem> keyboard = ::engine::Engine::Get().GetSystem<engine::KeyboardSystem>();
    static Opt<engine::MouseSystem> mouse = ::engine::Engine::Get().GetSystem<MouseSystem>();
}

void EditorGroupSystem::OnEditorModeChanged(map::EditorModeChangedEvent const& Evt)
{
}

void EditorGroupSystem::OnEditorBack( map::EditorBackEvent const& Evt )
{
}

void EditorGroupSystem::EnableSubsystems( bool enable )
{
}

Opt<EditorGroupSystem> EditorGroupSystem::Get()
{
    return engine::Engine::Get().GetSystem<EditorGroupSystem>();
}


void EditorGroupSystem::OnNewGroup()
{
    int max = 0;
    for (auto groupName : mGroupNames)
    {
        try
        {
            if (boost::starts_with( groupName, "g" ))
            {
                max = std::max(std::stoi( groupName.substr(1) ),max);
            }
        }
        catch (...)
        {
        }
    }
    ++max;
    std::string newGroupName = "g" + std::to_string( max );
    mGroupNames.push_back( newGroupName );
    auto groupMapElement = MapElementFactory::Get()(AutoId("group"));
    groupMapElement->SetUID( AutoId(newGroupName) );
    MapSystem::Get()->GetMapElementList().insert( groupMapElement.release() );
    EventServer<GroupSelectedEvent>::Get().SendEvent( GroupSelectedEvent(newGroupName) );
    EventServer<EditorBackEvent>::Get().SendEvent( EditorBackEvent() );
}

std::vector<std::string> EditorGroupSystem::GetGroupNames() const
{
    return mGroupNames;
}

void EditorGroupSystem::SetGroupNames( std::vector<std::string> val )
{
    mGroupNames = val;
}

void EditorGroupSystem::OnGroupSelected( std::string groupName )
{
    EventServer<GroupSelectedEvent>::Get().SendEvent( GroupSelectedEvent( groupName ) );
    EventServer<EditorBackEvent>::Get().SendEvent( EditorBackEvent() );
}

} // namespace map

