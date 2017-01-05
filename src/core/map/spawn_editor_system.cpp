#include "platform/i_platform.h"
#include "spawn_editor_system.h"
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
#include "level_generator/spawn_property.h"
#include "level_generator/property_factory.h"

namespace map {

SpawnEditorSystem::SpawnEditorSystem()
    : PropertyEditorBaseSystem("editor/spawn_hud","spawn")
    , mScene( Scene::Get() )
    , mEditorPropModel( "spawn", &RootModel::Get() )
    , mNewPropModel( VoidFunc( this, &SpawnEditorSystem::OnNewProp ), "new_prop", &mEditorPropModel )
    , mPropNamesModel( (ModelValue::get_string_vec_t) RefTo( mPropNames ), "props", &mEditorPropModel )
    , mPropSelectedModel( StringFunc( this, &SpawnEditorSystem::OnPropSelected ), "prop_selected", &mEditorPropModel )
    , mTargetNameModel( (ModelValue::get_string_t) RefTo( mTargetName ), "target", &mEditorPropModel )
    , mChangeTargetModel( VoidFunc( this, &SpawnEditorSystem::OnChangeTarget ), "change_target", &mEditorPropModel )
    , mChancePressModel( StringFunc( this, &SpawnEditorSystem::ChancePress ), "chance_press", &mEditorPropModel )
    , mChanceModel( RefTo( mChance ), "chance", &mEditorPropModel )
{
}


void SpawnEditorSystem::Init()
{
    PropertyEditorBaseSystem::Init();
    mOnEditorModeChanged = EventServer<map::EditorModeChangedEvent>::Get().Subscribe( boost::bind( &SpawnEditorSystem::OnEditorModeChanged, this, _1 ) );
    mOnEditorBack = EventServer<map::EditorBackEvent>::Get().Subscribe( boost::bind( &SpawnEditorSystem::OnEditorBack, this, _1 ) );
    mOnGroupSelected = EventServer<map::GroupSelectedEvent>::Get().Subscribe( boost::bind( &SpawnEditorSystem::OnGroupSelected, this, _1 ) );
}

void SpawnEditorSystem::Update(double DeltaTime)
{
    if (!mEnabled)
    {
        return;
    }
    HandleSpace();
}

void SpawnEditorSystem::OnEditorModeChanged(map::EditorModeChangedEvent const& Evt)
{
    HandleModeChanged( Evt );
}

void SpawnEditorSystem::OnEditorBack( map::EditorBackEvent const& Evt )
{
    if (mEnabled)
    {
        EditorSelectSystem::SetActorColors( mGroupPreSelectedActors, nullptr );
        mGroupPreSelectedActors.clear();
        mGroupPreSelectName.clear();
        HandleEditorBack( Evt );
    }
}

void SpawnEditorSystem::EnableSubsystems( bool enable )
{
}

Opt<SpawnEditorSystem> SpawnEditorSystem::Get()
{
    return engine::Engine::Get().GetSystem<SpawnEditorSystem>();
}


void SpawnEditorSystem::OnNewProp()
{
    static auto& propertyFactory = PropertyFactory::Get();
    static int32_t propertyId = AutoId("spawn");
    auto prop = propertyFactory( propertyId );
    mPropUID = prop->GetUID();
    mTargetName.clear();
    mRoomDesc->GetRoom()->AddProperty( prop );
    LoadProp();
    LoadPropNames();
    Ui::Get().Load( "editor/spawn_edit" );
}

void SpawnEditorSystem::OnPropSelected( std::string groupName )
{
    mTargetName.clear();
    mPropUID = -1;
    try
    {
        mPropUID = std::stoi( groupName.substr( 4 ) );
    }
    catch (...) {}
    LoadProp();
    Ui::Get().Load( "editor/spawn_edit" );
}

void SpawnEditorSystem::OnChangeTarget()
{
    Ui::Get().Load( "editor/all_groups" );
}

void SpawnEditorSystem::OnGroupSelected( map::GroupSelectedEvent const& Evt )
{
    if (!mEnabled)
    {
        return;
    }
    if (mGroupPreSelectName == Evt.mGroupName)
    {
        EditorSelectSystem::SetActorColors( mGroupPreSelectedActors, nullptr );
        mGroupPreSelectedActors.clear();
        mGroupPreSelectName.clear();
        Opt<SpawnProperty> prop = mRoomDesc->GetRoom()->GetProperty( mPropUID );
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
        Ui::Get().Load( "editor/spawn_edit" );
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

void SpawnEditorSystem::ChancePress( std::string modifier )
{
    static Opt<engine::KeyboardSystem> keyboard = ::engine::Engine::Get().GetSystem<engine::KeyboardSystem>();
    bool addition = modifier == "+";
    int32_t ammount = 1;
    if (keyboard->GetKey( GLFW_KEY_LEFT_SHIFT ).State == KeyState::Down
        || keyboard->GetKey( GLFW_KEY_RIGHT_SHIFT ).State == KeyState::Down)
    {
        ammount = 10;
    }
    if (keyboard->GetKey( GLFW_KEY_LEFT_CONTROL ).State == KeyState::Down
        || keyboard->GetKey( GLFW_KEY_RIGHT_CONTROL ).State == KeyState::Down)
    {
        ammount = 50;
    }
    
    if (addition)
    {
        mChance = std::min( mChance + ammount, 100 );
    }
    else
    {
        mChance = std::max( mChance - ammount, 1 );
    }
    Opt<SpawnProperty> prop = mRoomDesc->GetRoom()->GetProperty( mPropUID );
    if (prop.IsValid())
    {
        prop->SetChance( mChance );
    }
}

void SpawnEditorSystem::LoadProp()
{
    static auto& idStorage = IdStorage::Get();
    Opt<SpawnProperty> prop = mRoomDesc->GetRoom()->GetProperty( mPropUID );
    if (prop.IsValid())
    {
        auto const& targets = prop->GetTargets();
        if (!targets.empty()) // TODO: handle more then one target. Just a reminder the editor is a helper to edit json files.
        {
            idStorage.GetName( *targets.begin(), mTargetName );
        }
        mChance = prop->GetChance();
    }
}

} // namespace map

