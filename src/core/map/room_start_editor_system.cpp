#include "platform/i_platform.h"
#include "room_start_editor_system.h"
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
#include "level_generator/room_start_property.h"
#include "level_generator/property_factory.h"

namespace map {

RoomStartEditorSystem::RoomStartEditorSystem()
    : PropertyEditorBaseSystem( "editor/room_start_hud", "room_start" )
    , mScene( Scene::Get() )
    , mEditorPropModel( "room_start", &RootModel::Get() )
    , mNewPropModel( VoidFunc( this, &RoomStartEditorSystem::OnNewProp ), "new_prop", &mEditorPropModel )
    , mPropNamesModel( (ModelValue::get_string_vec_t) RefTo( mPropNames ), "props", &mEditorPropModel )
    , mPropSelectedModel( StringFunc( this, &RoomStartEditorSystem::OnPropSelected ), "prop_selected", &mEditorPropModel )
    , mTargetNameModel( (ModelValue::get_string_t) RefTo( mTargetName ), "target", &mEditorPropModel )
    , mChangeTargetModel( VoidFunc( this, &RoomStartEditorSystem::OnChangeTarget ), "change_target", &mEditorPropModel )
{
}


void RoomStartEditorSystem::Init()
{
    PropertyEditorBaseSystem::Init();
    mOnEditorModeChanged = EventServer<map::EditorModeChangedEvent>::Get().Subscribe( boost::bind( &RoomStartEditorSystem::OnEditorModeChanged, this, _1 ) );
    mOnEditorBack = EventServer<map::EditorBackEvent>::Get().Subscribe( boost::bind( &RoomStartEditorSystem::OnEditorBack, this, _1 ) );
    mOnGroupSelected = EventServer<map::GroupSelectedEvent>::Get().Subscribe( boost::bind( &RoomStartEditorSystem::OnGroupSelected, this, _1 ) );
}

void RoomStartEditorSystem::Update(double DeltaTime)
{
    if (!mEnabled)
    {
        return;
    }
    static Opt<engine::KeyboardSystem> keyboard = ::engine::Engine::Get().GetSystem<engine::KeyboardSystem>();
    static Opt<engine::MouseSystem> mouse = ::engine::Engine::Get().GetSystem<MouseSystem>();
    HandleSpace();
}

void RoomStartEditorSystem::OnEditorModeChanged(map::EditorModeChangedEvent const& Evt)
{
    HandleModeChanged( Evt );
}

void RoomStartEditorSystem::OnEditorBack( map::EditorBackEvent const& Evt )
{
    if (mEnabled)
    {
        EditorSelectSystem::SetActorColors( mGroupPreSelectedActors, nullptr );
        mGroupPreSelectedActors.clear();
        mGroupPreSelectName.clear();
        HandleEditorBack( Evt );
    }
}

void RoomStartEditorSystem::EnableSubsystems( bool enable )
{
}

Opt<RoomStartEditorSystem> RoomStartEditorSystem::Get()
{
    return engine::Engine::Get().GetSystem<RoomStartEditorSystem>();
}


void RoomStartEditorSystem::OnNewProp()
{
    static auto& propertyFactory = PropertyFactory::Get();
    static int32_t propertyId = AutoId("room_start");
    auto prop = propertyFactory( propertyId );
    mPropUID = prop->GetUID();
    mTargetName.clear();
    mRoomDesc->GetRoom()->AddProperty( prop );
    LoadProp();
    LoadPropNames();
    Ui::Get().Load( "editor/room_start_edit" );
}

void RoomStartEditorSystem::OnPropSelected( std::string groupName )
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
    Ui::Get().Load( "editor/room_start_edit" );
}

void RoomStartEditorSystem::OnChangeTarget()
{
    Ui::Get().Load( "editor/all_groups" );
}

void RoomStartEditorSystem::OnGroupSelected( map::GroupSelectedEvent const& Evt )
{
    if (!mEnabled)
    {
        return;
    }
    if (mGroupPreSelectName == Evt.mGroupName)
    {
        EditorSelectSystem::SetActorColors( mGroupPreSelectedActors, nullptr );
        mGroupPreSelectedActors.clear();
        Opt<RoomStartProperty> prop = mRoomDesc->GetRoom()->GetProperty( mPropUID );
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
        Ui::Get().Load( "editor/room_start_edit" );
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

void RoomStartEditorSystem::LoadProp()
{
    static auto& idStorage = IdStorage::Get();
    Opt<RoomStartProperty> prop = mRoomDesc->GetRoom()->GetProperty( mPropUID );
    if (prop.IsValid())
    {
        auto const& targets = prop->GetTargets();
        if (!targets.empty()) // TODO: handle more then one target. Just a reminder the editor is a helper to edit json files.
        {
            idStorage.GetName( *targets.begin(), mTargetName );
        }
    }
}

} // namespace map

