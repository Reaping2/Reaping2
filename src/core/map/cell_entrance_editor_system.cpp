#include "platform/i_platform.h"
#include "cell_entrance_editor_system.h"
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
#include "level_generator/cell_entrance_property.h"
#include "level_generator/property_factory.h"
#include "room_cell_editor_system.h"

namespace map {

CellEntranceEditorSystem::CellEntranceEditorSystem()
    : PropertyEditorBaseSystem( "editor/cell_entrance_hud", "cell_entrance" )
    , mScene( Scene::Get() )
    , mEditorPropModel( "cell_entrance", &RootModel::Get() )
    , mNewPropModel( VoidFunc( this, &CellEntranceEditorSystem::OnNewProp ), "new_prop", &mEditorPropModel )
    , mPropNamesModel( (ModelValue::get_string_vec_t) RefTo( mPropNames ), "props", &mEditorPropModel )
    , mPropSelectedModel( StringFunc( this, &CellEntranceEditorSystem::OnPropSelected ), "prop_selected", &mEditorPropModel )
    , mTargetNameModel( (ModelValue::get_string_t) RefTo( mTargetName ), "target", &mEditorPropModel )
    , mChangeTargetModel( VoidFunc( this, &CellEntranceEditorSystem::OnChangeTarget ), "change_target", &mEditorPropModel )
    , mBlockedTargetNameModel( (ModelValue::get_string_t) RefTo( mBlockedTargetName ), "blocked_target", &mEditorPropModel )
    , mChangeBlockedTargetModel( VoidFunc( this, &CellEntranceEditorSystem::OnChangeBlockedTarget ), "change_blocked_target", &mEditorPropModel )
{
}


void CellEntranceEditorSystem::Init()
{
    PropertyEditorBaseSystem::Init();
    mOnEditorModeChanged = EventServer<map::EditorModeChangedEvent>::Get().Subscribe( boost::bind( &CellEntranceEditorSystem::OnEditorModeChanged, this, _1 ) );
    mOnEditorBack = EventServer<map::EditorBackEvent>::Get().Subscribe( boost::bind( &CellEntranceEditorSystem::OnEditorBack, this, _1 ) );
    mOnGroupSelected = EventServer<map::GroupSelectedEvent>::Get().Subscribe( boost::bind( &CellEntranceEditorSystem::OnGroupSelected, this, _1 ) );
    mMouseMoveId = EventServer<WorldMouseMoveEvent>::Get().Subscribe( boost::bind( &CellEntranceEditorSystem::OnMouseMoveEvent, this, _1 ) );
}

void CellEntranceEditorSystem::Update(double DeltaTime)
{
    if (!mEnabled)
    {
        return;
    }
    static Opt<engine::KeyboardSystem> keyboard = ::engine::Engine::Get().GetSystem<engine::KeyboardSystem>();
    static Opt<engine::MouseSystem> mouse = ::engine::Engine::Get().GetSystem<MouseSystem>();
    if (keyboard->GetKey( GLFW_KEY_SPACE ).State == KeyState::Typed)
    {
        if (mEditorMode == PropSelect)
        {
            
        }
        else
        {
            mEditorMode = PropSelect;
            EditorHudState::Get().SetHudShown( false );
            Ui::Get().Load( mHUD );
        }
    }
    if (mEditorMode == PropSelect
        && engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Left ))
    {
        mPropUID = GetPropUIDFromScene( mMousePos );
        if (mPropUID == -1)
        {
            mPropUID = AddNewProperty();
        }
        LoadProp();
        mEditorMode = Edit;
        EditorHudState::Get().SetHudShown( true );
        Ui::Get().Load( "editor/cell_entrance_edit" );
    }
}


int32_t CellEntranceEditorSystem::AddNewProperty()
{
    static auto& propertyFactory = PropertyFactory::Get();
    static int32_t propertyId = AutoId( "cell_entrance" );
    auto prop = propertyFactory( propertyId );
    int32_t r = prop->GetUID();
    mTargetName.clear();
    mBlockedTargetName.clear();
    Opt<CellEntranceProperty> cellEntranceProp = static_cast<CellEntranceProperty*>(prop.get());
    static auto roomCellEditorSystem = RoomCellEditorSystem::Get();
    auto cell = roomCellEditorSystem->GetCellFromScene( mMousePos );
    auto entrance = roomCellEditorSystem->GetEntranceType( mMousePos );
    cellEntranceProp->SetX( cell->mDescCoord.x );
    cellEntranceProp->SetY( cell->mDescCoord.y );
    cellEntranceProp->SetEntranceType( entrance );

    mRoomDesc->GetRoom()->AddProperty( prop );            
    return r;
}

int32_t CellEntranceEditorSystem::GetPropUIDFromScene( glm::vec2 mousePos )
{
    static auto roomCellEditorSystem = RoomCellEditorSystem::Get();
    auto cell = roomCellEditorSystem->GetCellFromScene( mousePos );
    auto entrance = roomCellEditorSystem->GetEntranceType( mousePos );
    if (cell.IsValid())
    {
        for (auto&& prop : mRoomDesc->GetRoom()->GetProperties())
        {
            if (prop.GetType() == CellEntranceProperty::GetType_static())
            {
                auto& cellEntranceProp = static_cast<CellEntranceProperty&>(prop);
                if (cellEntranceProp.GetX() == cell->mDescCoord.x
                    &&cellEntranceProp.GetY() == cell->mDescCoord.y
                    &&cellEntranceProp.GetEntranceType() == entrance)
                {
                    return prop.GetUID();
                }
            }
        }
    }
    return -1;
}

void CellEntranceEditorSystem::OnEditorModeChanged( map::EditorModeChangedEvent const& Evt )
{
    if (Evt.mMode == mMode)
    {
        ::engine::Engine::Get().SetEnabled<CellEntranceEditorSystem>( true );
        Ui::Get().Load( mHUD );
        RoomCellEditorSystem::Get()->AddCells();
        EditorHudState::Get().SetHudShown( false );
        mEditorMode = PropSelect;
    }
    else
    {
        if (Evt.mPrevMode == mMode)
        { 
            if (!RoomCellEditorSystem::Get()->IsCellsVisible())
            {
                RoomCellEditorSystem::Get()->RemoveCells();
            }
        }
        ::engine::Engine::Get().SetEnabled<CellEntranceEditorSystem>( false );
    }

}

void CellEntranceEditorSystem::OnEditorBack( map::EditorBackEvent const& Evt )
{
    if (mEnabled)
    {
        EditorSelectSystem::SetActorColors( mGroupPreSelectedActors, nullptr );
        mGroupPreSelectedActors.clear();
        mGroupPreSelectName.clear();
        HandleEditorBack( Evt );
    }
}

void CellEntranceEditorSystem::EnableSubsystems( bool enable )
{
}

Opt<CellEntranceEditorSystem> CellEntranceEditorSystem::Get()
{
    return engine::Engine::Get().GetSystem<CellEntranceEditorSystem>();
}


void CellEntranceEditorSystem::OnNewProp()
{
}

void CellEntranceEditorSystem::OnPropSelected( std::string groupName )
{
}

void CellEntranceEditorSystem::OnChangeTarget()
{
    mEditorMode = EntranceTarget;
    Ui::Get().Load( "editor/all_groups" );
}

void CellEntranceEditorSystem::OnChangeBlockedTarget()
{
    mEditorMode = BlockedTarget;
    Ui::Get().Load( "editor/all_groups" );
}

void CellEntranceEditorSystem::OnGroupSelected( map::GroupSelectedEvent const& Evt )
{
    if (!mEnabled)
    {
        return;
    }
    if (mGroupPreSelectName == Evt.mGroupName)
    {
        EditorSelectSystem::SetActorColors( mGroupPreSelectedActors, nullptr );
        mGroupPreSelectedActors.clear();
        Opt<CellEntranceProperty> prop = mRoomDesc->GetRoom()->GetProperty( mPropUID );
        if (prop.IsValid())
        {
            CellEntranceProperty::Targets_t targets = mEditorMode == EntranceTarget ? prop->GetEntranceTargets() : prop->GetBlockedTargets();;
            int32_t groupId = AutoId( Evt.mGroupName );
            if (targets.empty())
            {
                targets.push_back( groupId );
            }
            else
            {
                targets.at( 0 ) = groupId;
            }
            if (mEditorMode == EntranceTarget)
            {
                prop->SetEntranceTargets( targets );
                mTargetName = Evt.mGroupName;
            }
            else
            {
                prop->SetBlockedTargets( targets );
                mBlockedTargetName = Evt.mGroupName;
            }
        }
        EditorSelectSystem::SetActorColors( mGroupPreSelectedActors, nullptr );
        mGroupPreSelectedActors.clear();
        mGroupPreSelectName.clear();
        mEditorMode = Edit;
        Ui::Get().Load( "editor/cell_entrance_edit" );
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


void CellEntranceEditorSystem::OnMouseMoveEvent( const WorldMouseMoveEvent& Event )
{
    mMousePos = Event.Pos;
}

void CellEntranceEditorSystem::LoadProp()
{
    static auto& idStorage = IdStorage::Get();
    Opt<CellEntranceProperty> prop = mRoomDesc->GetRoom()->GetProperty( mPropUID );
    if (prop.IsValid())
    {
        auto const& targets = prop->GetEntranceTargets();
        if (!targets.empty()) // TODO: handle more then one target. Just a reminder the editor is a helper to edit json files.
        {
            idStorage.GetName( *targets.begin(), mTargetName );
        }
        auto const& blockedTargets = prop->GetBlockedTargets();
        if (!blockedTargets.empty()) // TODO: handle more then one target. Just a reminder the editor is a helper to edit json files.
        {
            idStorage.GetName( *blockedTargets.begin(), mBlockedTargetName );
        }
    }
}

} // namespace map

