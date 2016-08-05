#include "platform/i_platform.h"
#include "room_cell_editor_system.h"
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

namespace map {

RoomCellEditorSystem::RoomCellEditorSystem()
    : mScene( Scene::Get() )
    , mCellEditorModel( "cell_editor", &RootModel::Get() )
    , mCellCountPressModel( StringFunc( this, &RoomCellEditorSystem::CellCountPress ), "cell_count_press", &mCellEditorModel )
    , mCellCountModel( RefTo(mCellCount), "cell_count", &mCellEditorModel )
    , mCellSizePressModel( StringFunc( this, &RoomCellEditorSystem::CellSizePress ), "cell_size_press", &mCellEditorModel )
    , mCellSizeModel( RefTo( mCellSize ), "cell_size", &mCellEditorModel )
    , mMouseModeModel( StringFunc( this, &RoomCellEditorSystem::SetMouseMode ), "mode", &mCellEditorModel )
{
}


void RoomCellEditorSystem::Init()
{
    mOnEditorModeChanged = EventServer<map::EditorModeChangedEvent>::Get().Subscribe( boost::bind( &RoomCellEditorSystem::OnEditorModeChanged, this, _1 ) );
    mOnEditorBack = EventServer<map::EditorBackEvent>::Get().Subscribe( boost::bind( &RoomCellEditorSystem::OnEditorBack, this, _1 ) );
    mMouseMoveId = EventServer<WorldMouseMoveEvent>::Get().Subscribe( boost::bind( &RoomCellEditorSystem::OnMouseMoveEvent, this, _1 ) );
    mOnRoomEditorLoaded = EventServer<map::RoomEditorLoadedEvent>::Get().Subscribe( boost::bind( &RoomCellEditorSystem::OnRoomEditorLoaded, this, _1 ) );
}


void RoomCellEditorSystem::Update(double DeltaTime)
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
        }
        else
        {
            EnableSubsystems( false );
            Ui::Get().Load( "room_cell_editor_hud" );
            EditorHudState::Get().SetHudShown( true );
        }
    }
    if (!EditorHudState::Get().IsHudShown())
    {
        if (keyboard->GetKey( GLFW_KEY_F ).State == KeyState::Typed
            || keyboard->GetKey( GLFW_KEY_KP_5 ).State == KeyState::Typed)
        {
            SwitchCellFilledState( mMousePos );
        }
        if (keyboard->GetKey( GLFW_KEY_KP_8 ).State == KeyState::Typed)
        {
            SwitchEntranceState( mMousePos, EntranceType::Top );
        }
        if (keyboard->GetKey( GLFW_KEY_KP_6 ).State == KeyState::Typed)
        {
            SwitchEntranceState( mMousePos, EntranceType::Right );
        }
        if (keyboard->GetKey( GLFW_KEY_KP_2 ).State == KeyState::Typed)
        {
            SwitchEntranceState( mMousePos, EntranceType::Bottom );
        }
        if (keyboard->GetKey( GLFW_KEY_KP_4 ).State == KeyState::Typed)
        {
            SwitchEntranceState( mMousePos, EntranceType::Left );
        }
        if (keyboard->GetKey( GLFW_KEY_KP_ADD ).State == KeyState::Typed)
        {
            if (keyboard->GetKey( GLFW_KEY_KP_ADD ).Mods&GLFW_MOD_SHIFT)
            {
                CellSizePress( "+" );
            }
            else
            {
                CellCountPress( "+" );
            }
        }
        if (keyboard->GetKey( GLFW_KEY_KP_SUBTRACT ).State == KeyState::Typed)
        {
            if (keyboard->GetKey( GLFW_KEY_KP_SUBTRACT ).Mods&GLFW_MOD_SHIFT)
            {
                CellSizePress( "-" );
            }
            else
            {
                CellCountPress( "-" );
            }
        }

        if (mMouseLeftPressed && !engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Left ))
        {
            if (mMouseMode == CellFill)
            {
                SwitchCellFilledState( mMousePos );
            }
            else if (mMouseMode == CellEntrance)
            {
                if (mRoomDesc.IsValid())
                {
                    SwitchEntranceState( mMousePos, GetEntranceType(mMousePos) );
                }
            }
            mMouseLeftPressed = false;
        }
        else if (engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Left ))
        {
            mMouseLeftPressed = true;
        }

        if (mMouseRightPressed && !engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Right ))
        {
            mMouseRightPressed = false;
        }
        else if (engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Right ))
        {
            mMouseRightPressed = true;
        }
    }
}

void RoomCellEditorSystem::OnEditorModeChanged(map::EditorModeChangedEvent const& Evt)
{
    if (Evt.mMode == "cell")
    {
        EnableSubsystems( false );
        ::engine::Engine::Get().SetEnabled<RoomCellEditorSystem>( true );
        Ui::Get().Load( "room_cell_editor_hud" );
        EditorHudState::Get().SetHudShown( true );
        AddCells();
    }
    else if (Evt.mPrevMode == "cell")
    {
        EnableSubsystems( false );
        ::engine::Engine::Get().SetEnabled<RoomCellEditorSystem>( false );
        RemoveCells();
    }
}

void RoomCellEditorSystem::OnEditorBack( map::EditorBackEvent const& Evt )
{
    if (mEnabled)
    {
        if (!Evt.mBackToBaseHud)
        {
            EnableSubsystems( false );
            Ui::Get().Load( "room_cell_editor_hud" );
            EditorHudState::Get().SetHudShown( true );
        }
    }
}

void RoomCellEditorSystem::EnableSubsystems( bool enable )
{
}

void RoomCellEditorSystem::RemoveCells()
{
    for (auto cellGUID : mCellGUIDs)
    {
        mScene.RemoveActor( cellGUID );
    }
}

void RoomCellEditorSystem::AddCells()
{
    static ActorFactory& actorFactory = ActorFactory::Get();
    static int32_t cellId = AutoId( "cell" );
    RemoveCells();
    mCellGUIDs.clear();
    if (!mRoomDesc.IsValid())
    {
        return;
    }
    for (int32_t y = 0; y < mRoomDesc->GetCellCount(); ++y)
    {
        for (int32_t x = 0; x < mRoomDesc->GetCellCount(); ++x)
        {
            std::auto_ptr<Actor> cellActor( actorFactory( cellId ) );
            Opt<IPositionComponent> positionC( cellActor->Get<IPositionComponent>() );
            if (positionC.IsValid())
            {
                positionC->SetX( mRoomDesc->GetCellSize() * x + mRoomDesc->GetCellSize() / 2 );
                positionC->SetY( mRoomDesc->GetCellSize() * y + mRoomDesc->GetCellSize() / 2 );
            }
            auto CellC( cellActor->Get<ICellComponent>() );
            mCellGUIDs.push_back( cellActor->GetGUID() );
            if (CellC.IsValid())
            {
                CellC->SetRoomDesc( mRoomDesc );
                CellC->SetX( x );
                CellC->SetY( y );
            }
            auto CollisionC( cellActor->Get<ICollisionComponent>() );
            if (CollisionC.IsValid())
            {
                CollisionC->SetRadius( mRoomDesc->GetCellSize() / 2 );
            }

            mScene.AddActor( cellActor.release() );
        }
    }
}

Opt<RoomCellEditorSystem> RoomCellEditorSystem::Get()
{
    return engine::Engine::Get().GetSystem<RoomCellEditorSystem>();
}

void RoomCellEditorSystem::SetRoomDesc( Opt<RoomDesc> roomDesc )
{
    mRoomDesc = roomDesc;
    if (mRoomDesc.IsValid())
    {
        mCellCount = mRoomDesc->GetCellCount();
        mCellSize = mRoomDesc->GetCellSize()/100;
    }
}

void RoomCellEditorSystem::CellCountPress( std::string modifier )
{
    if (!mRoomDesc.IsValid())
    {
        return;
    }
    bool addition = modifier == "+";
    auto cellCount = mRoomDesc->GetCellCount();
    if (addition)
    {
        cellCount = std::min( cellCount + 1, 20 );
    }
    else
    {
        cellCount = std::max( cellCount -1, 1 );
    }
    mRoomDesc->SetCellCount( cellCount );
    mCellCount = mRoomDesc->GetCellCount();
    AddCells();
}

void RoomCellEditorSystem::CellSizePress( std::string modifier )
{
    if (!mRoomDesc.IsValid())
    {
        return;
    }
    bool addition = modifier == "+";
    auto cellSize = mRoomDesc->GetCellSize();
    if (addition)
    {
        cellSize = std::min( cellSize + 100, 2000 );
    }
    else
    {
        cellSize = std::max( cellSize - 100, 400 );
    }
    mRoomDesc->SetCellSize( cellSize );
    mCellSize = mRoomDesc->GetCellSize()/100;
    AddCells();
}

void RoomCellEditorSystem::OnMouseMoveEvent( const WorldMouseMoveEvent& Event )
{
    mMousePos = Event.Pos;
}

Opt<Cell> RoomCellEditorSystem::GetCellFromScene( glm::vec2 pos )
{
    Opt<Cell> r;
    if (pos.x < 0 || pos.y < 0 || !mRoomDesc.IsValid())
    {
        return r;
    }
    int32_t x = pos.x / mRoomDesc->GetCellSize();
    int32_t y = pos.y / mRoomDesc->GetCellSize();
    if (x >= mRoomDesc->GetCellCount() || y >= mRoomDesc->GetCellCount())
    {
        return r;
    }
    r = &mRoomDesc->GetCell( x, y );
    return r;
}

void RoomCellEditorSystem::SwitchEntranceState( glm::vec2 pos, EntranceType::Type entrance )
{
    Opt<Cell> cell = GetCellFromScene( pos );
    if (cell.IsValid())
    {
        if (cell->HasEntrance( entrance ))
        {
            cell->RemoveEntrance( entrance );
        }
        else
        {
            cell->AddEntrance( entrance );
        }
    }
}

void RoomCellEditorSystem::SetMouseMode( std::string mode )
{
    if (mode == "nothing")
    {
        mMouseMode = Nothing;
    }
    else if (mode == "entrance")
    {
        mMouseMode = CellEntrance;
    }
    else if (mode == "fill")
    {
        mMouseMode = CellFill;
    }
    EventServer<EditorBackEvent>::Get().SendEvent( EditorBackEvent( true ) );
}

void RoomCellEditorSystem::SwitchCellFilledState( glm::vec2 pos )
{
    Opt<Cell> cell = GetCellFromScene( mMousePos );
    if (cell.IsValid())
    {
        cell->SetFilled( !cell->IsFilled() );
    }
}

void RoomCellEditorSystem::OnRoomEditorLoaded( map::RoomEditorLoadedEvent const& Evt )
{
    mRoomDesc = Evt.mRoomDesc;
}

EntranceType::Type RoomCellEditorSystem::GetEntranceType( glm::vec2 pos )
{
    int32_t x = (int)pos.x%mRoomDesc->GetCellSize();
    int32_t y = (int)pos.y%mRoomDesc->GetCellSize();
    if (x < mRoomDesc->GetCellSize()*0.3)
    {
        return EntranceType::Left;
    }
    else if (x > mRoomDesc->GetCellSize()*0.7)
    {
        return  EntranceType::Right;
    }
    if (y < mRoomDesc->GetCellSize()*0.3)
    {
        return  EntranceType::Bottom;
    }
    else if (y > mRoomDesc->GetCellSize()*0.7)
    {
        return  EntranceType::Top;
    }

}

} // namespace map

