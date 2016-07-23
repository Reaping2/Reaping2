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

namespace map {

RoomCellEditorSystem::RoomCellEditorSystem()
    : mScene( Scene::Get() )
{
}


void RoomCellEditorSystem::Init()
{
    mOnEditorModeChanged = EventServer<map::EditorModeChangedEvent>::Get().Subscribe( boost::bind( &RoomCellEditorSystem::OnEditorModeChanged, this, _1 ) );
    mOnEditorBack = EventServer<map::EditorBackEvent>::Get().Subscribe( boost::bind( &RoomCellEditorSystem::OnEditorBack, this, _1 ) );
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
    else
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
            mScene.AddActor( cellActor.release() );
            if (CellC.IsValid())
            {
                CellC->SetRoomDesc( mRoomDesc );
                CellC->SetX( x );
                CellC->SetY( y );
            }
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
}

} // namespace map

