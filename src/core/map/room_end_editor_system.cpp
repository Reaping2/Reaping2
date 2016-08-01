#include "platform/i_platform.h"
#include "room_end_editor_system.h"
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
#include "core/map/level_generator/i_room.h"
#include "level_generator/room_end_property.h"
#include "level_generator/property_factory.h"

namespace map {

RoomEndEditorSystem::RoomEndEditorSystem()
    : PropertyEditorBaseSystem( "editor/room_end_hud", "room_end" )
    , mScene( Scene::Get() )
    , mEditorPropModel( "room_end", &RootModel::Get() )
    , mNewPropModel( VoidFunc( this, &RoomEndEditorSystem::OnNewProp ), "new_prop", &mEditorPropModel )
    , mPropNamesModel( (ModelValue::get_string_vec_t) RefTo( mPropNames ), "props", &mEditorPropModel )
    , mPropSelectedModel( StringFunc( this, &RoomEndEditorSystem::OnPropSelected ), "prop_selected", &mEditorPropModel )
    , mTargetNameModel( (ModelValue::get_string_t) RefTo( mTargetName ), "target", &mEditorPropModel )
    , mChangeTargetModel( VoidFunc( this, &RoomEndEditorSystem::OnChangeTarget ), "change_target", &mEditorPropModel )
{
}


void RoomEndEditorSystem::Init()
{
    mOnEditorModeChanged = EventServer<map::EditorModeChangedEvent>::Get().Subscribe( boost::bind( &RoomEndEditorSystem::OnEditorModeChanged, this, _1 ) );
    mOnEditorBack = EventServer<map::EditorBackEvent>::Get().Subscribe( boost::bind( &RoomEndEditorSystem::OnEditorBack, this, _1 ) );
    mOnRoomEditorLoaded = EventServer<map::RoomEditorLoadedEvent>::Get().Subscribe( boost::bind( &RoomEndEditorSystem::OnRoomEditorLoaded, this, _1 ) );
    mOnGroupSelected = EventServer<map::GroupSelectedEvent>::Get().Subscribe( boost::bind( &RoomEndEditorSystem::OnGroupSelected, this, _1 ) );
}

void RoomEndEditorSystem::OnRoomEditorLoaded( map::RoomEditorLoadedEvent const& Evt )
{
    mRoomDesc = Evt.mRoomDesc;
    LoadRoomDesc();
}

void RoomEndEditorSystem::Update(double DeltaTime)
{
    if (!mEnabled)
    {
        return;
    }
    static Opt<engine::KeyboardSystem> keyboard = ::engine::Engine::Get().GetSystem<engine::KeyboardSystem>();
    static Opt<engine::MouseSystem> mouse = ::engine::Engine::Get().GetSystem<MouseSystem>();
    HandleSpace();
}

void RoomEndEditorSystem::OnEditorModeChanged(map::EditorModeChangedEvent const& Evt)
{
    HandleModeChanged( Evt );
}

void RoomEndEditorSystem::OnEditorBack( map::EditorBackEvent const& Evt )
{
    if (mEnabled)
    {
        EditorSelectSystem::SetActorColors( mGroupPreSelectedActors, nullptr );
        mGroupPreSelectedActors.clear();
        mGroupPreSelectName.clear();
        HandleEditorBack( Evt );
    }
}

void RoomEndEditorSystem::EnableSubsystems( bool enable )
{
}

Opt<RoomEndEditorSystem> RoomEndEditorSystem::Get()
{
    return engine::Engine::Get().GetSystem<RoomEndEditorSystem>();
}


void RoomEndEditorSystem::OnNewProp()
{
    static auto& propertyFactory = PropertyFactory::Get();
    static int32_t propertyId = AutoId("room_end");
    auto prop = propertyFactory( propertyId );
    mPropUID = prop->GetUID();
    mTargetName.clear();
    mRoomDesc->GetRoom()->AddProperty( prop );
    LoadProp();
    LoadRoomDesc();
    Ui::Get().Load( "editor/room_end_edit" );
}

void RoomEndEditorSystem::OnPropSelected( std::string groupName )
{
    mTargetName.clear();
    mPropUID = -1;
    static auto& idStorage = IdStorage::Get();
    try
    {
        mPropUID = std::stoi( groupName.substr( 4 ) );
    }
    catch (...) {}
    LoadProp();
    Ui::Get().Load( "editor/room_end_edit" );
}

void RoomEndEditorSystem::OnChangeTarget()
{
    Ui::Get().Load( "editor/all_groups" );
}

void RoomEndEditorSystem::OnGroupSelected( map::GroupSelectedEvent const& Evt )
{
    if (!mEnabled)
    {
        return;
    }
    if (mGroupPreSelectName == Evt.mGroupName)
    {
        EditorSelectSystem::SetActorColors( mGroupPreSelectedActors, nullptr );
        mGroupPreSelectedActors.clear();
        Opt<RoomEndProperty> prop = mRoomDesc->GetRoom()->GetProperty( mPropUID );
        if (prop.IsValid())
        {
            auto targets = prop->GetTargets();
            int32_t groupId = AutoId( Evt.mGroupName );
            if (targets.empty())
            {
                targets.push_back( groupId );
            }
            else
            {
                targets.at( 0 ) = groupId;
            }
            prop->SetTargets( targets );
            mTargetName = Evt.mGroupName;
        }
        EditorSelectSystem::SetActorColors( mGroupPreSelectedActors, nullptr );
        mGroupPreSelectedActors.clear();
        mGroupPreSelectName.clear();
        Ui::Get().Load( "editor/room_end_edit" );
    }
    else
    {
        mGroupPreSelectName = Evt.mGroupName;
        EditorSelectSystem::SetActorColors( mGroupPreSelectedActors, nullptr );
        mGroupPreSelectedActors.clear();
        EditorSelectSystem::FillActorColors( AutoId( Evt.mGroupName ), mGroupPreSelectedActors );
        EditorSelectSystem::SetActorColors( mGroupPreSelectedActors, &EditorSelectSystem::PreSelectColor );
    }
}

void RoomEndEditorSystem::LoadProp()
{
    static auto& idStorage = IdStorage::Get();
    Opt<RoomEndProperty> prop = mRoomDesc->GetRoom()->GetProperty( mPropUID );
    if (prop.IsValid())
    {
        auto const& targets = prop->GetTargets();
        if (!targets.empty()) // TODO: handle more then one target. Just a reminder the editor is a helper to edit json files.
        {
            idStorage.GetName( *targets.begin(), mTargetName );
        }
    }
}

void RoomEndEditorSystem::LoadRoomDesc()
{
    mPropNames.clear();
    if (!mRoomDesc.IsValid())
    {
        return;
    }

    for (auto&& prop : mRoomDesc->GetRoom()->GetProperties())
    {
        if (prop.GetType() == RoomEndProperty::GetType_static())
        {
            mPropNames.push_back( "prop" + std::to_string( prop.GetUID() ) );
        }
    }
}

} // namespace map

