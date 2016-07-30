#include "platform/i_platform.h"
#include "editor_group_system.h"
#include "ui/ui.h"
#include "editor_grid_system.h"
#include "editor_target_system.h"
#include "editor_brush_system.h"
#include "../../engine/engine.h"
#include "../../input/keyboard.h"
#include "editor_hud_state.h"
#include "input/mouse.h"
#include "group_selected_event.h"
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
        } catch (...) {}
    }
    ++max;
    std::string newGroupName = "g" + std::to_string( max );
    mGroupNames.push_back( newGroupName );
    auto groupMapElement = MapElementFactory::Get()(AutoId("group"));
    groupMapElement->SetIdentifier( AutoId(newGroupName) );
    MapSystem::Get()->GetMapElementList().insert( groupMapElement.release() );
    EventServer<GroupSelectedEvent>::Get().SendEvent( GroupSelectedEvent( newGroupName, true ) );
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
    EventServer<GroupSelectedEvent>::Get().SendEvent( GroupSelectedEvent( groupName, false ) );
}
// returns the uid for mapelement
int32_t EditorGroupSystem::RemoveFromAllGroups( int32_t actorGUID )
{
    int32_t r = -1;
    auto mapSystem = MapSystem::Get();
    auto mapElement( mapSystem->GetMapElement( actorGUID ) );
    if (!mapElement.IsValid())
    {
        return r; // this should not happen all actors should have a spawn actor map element at least
    }
    r = mapElement->GetUID();
    for (Opt<GroupMapElement> groupMapElement : MapElementListFilter<MapSystem::All>( mapSystem->GetMapElementList(), GroupMapElement::GetType_static() )) // all groups - most probably one
    {
        GroupMapElement::Targets_t targets = groupMapElement->GetTargets();
        targets.erase( mapElement->GetIdentifier() );
        groupMapElement->SetTargets( targets );
    }
    return r;
}

void EditorGroupSystem::AddToGroup( std::vector<int32_t> actorGUIDs, int32_t groupId )
{
    GroupMapElement::Targets_t targets;
    auto mapSystem = MapSystem::Get();
    for (auto actorGUID : actorGUIDs)
    {
        auto mapElement( mapSystem->GetMapElement( actorGUID ) );
        if (mapElement.IsValid())
        {
            targets.insert( mapElement->GetIdentifier() );
        }
    }
    for (Opt<GroupMapElement> groupMapElement : MapElementListFilter<MapSystem::Identifier>( mapSystem->GetMapElementList(), groupId )) // all groups - most probably one
    {
        auto newTargets = targets;
        newTargets.insert( groupMapElement->GetTargets().begin(), groupMapElement->GetTargets().end() );
        groupMapElement->SetTargets( newTargets );
    }
}

// returns the uid for mapelement when the actor is not present in other groups. -1 otherwise
int32_t EditorGroupSystem::RemoveFromGroup( int32_t actorGUID, int32_t groupId )
{
    auto mapSystem = MapSystem::Get();
    int32_t r = -1;
    auto mapElement( mapSystem->GetMapElement( actorGUID ) );
    if (!mapElement.IsValid())
    {
        return r; // this should not happen all actors should have a spawn actor map element at least
    }
    // check if there is another group containing this element if so the elements identifier remains the same. Otherwise it should be reset to spawn at start
    for (Opt<GroupMapElement> groupMapElement : MapElementListFilter<MapSystem::All>( mapSystem->GetMapElementList(), GroupMapElement::GetType_static() )) // all groups - most probably one
    {
        if (groupMapElement->GetIdentifier() == groupId)
        {
            GroupMapElement::Targets_t targets = groupMapElement->GetTargets();
            targets.erase( mapElement->GetIdentifier() );
            groupMapElement->SetTargets( targets );
        }
        else if (r==-1)
        {
            auto mapElement( mapSystem->GetMapElement( actorGUID ) );
            if (groupMapElement->GetTargets().find( mapElement->GetIdentifier() ) != groupMapElement->GetTargets().end())
            {
                r = mapElement->GetUID();
            }
        }
    }
    return r;
}

void EditorGroupSystem::SetMapElementIdentifier( int32_t mapElementUID, int32_t spawnID )
{
    auto mapSystem = MapSystem::Get();
    auto it = mapSystem->GetMapElementList().find( mapElementUID );
    if (it != mapSystem->GetMapElementList().end())
    {
        mapSystem->GetMapElementList().modify( it, MapElementIdentifierModifier( spawnID ) );
    }
}

} // namespace map

