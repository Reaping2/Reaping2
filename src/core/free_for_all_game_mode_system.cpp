#include "platform/i_platform.h"
#include "free_for_all_game_mode_system.h"
#include "network/lifecycle_message.h"
#include "network/client_datas_message.h"
#include "ui/ui.h"
#include "actor.h"
#include "actor_factory.h"
#include "i_position_component.h"
#include "i_inventory_component.h"
#include "player_controller_component.h"
#include "i_move_component.h"
#include "engine/engine.h"
#include "engine/soldier_spawn_system.h"
#include "engine/ctf_soldier_spawn_system.h"
#include "map/editor_system.h"
#include "map/editor_soldier_spawn_system.h"
#include "main/window.h"
#include "capture_the_flag_game_mode_system.h"
#include "rogue_game_mode_system.h"
#include "map/editor_target_system.h"
#include "map/editor_grid_system.h"
#include "map/editor_brush_system.h"
#include "map/room_editor_system.h"
#include "level_selection_system.h"
#include "map/level_generator/level_generated_event.h"

namespace core {

// TODO: subscribe to gamemodeselected event
FreeForAllGameModeSystem::FreeForAllGameModeSystem()
    : mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
{
}


void FreeForAllGameModeSystem::Init()
{
    mOnStartGameMode = EventServer<core::StartGameModeEvent>::Get().Subscribe( boost::bind( &FreeForAllGameModeSystem::OnStartGameMode, this, _1 ) );
    mOnLevelSelected = EventServer<core::LevelSelectedEvent>::Get().Subscribe( boost::bind( &FreeForAllGameModeSystem::OnLevelSelected, this, _1 ) );
}


void FreeForAllGameModeSystem::Update( double DeltaTime )
{
}

void FreeForAllGameModeSystem::OnStartGameMode( core::StartGameModeEvent const& Evt )
{
    if ( Evt.mMode != GameModes::FFA )
    {
        return;
    }
    ::engine::Engine::Get().SetEnabled< ::engine::SoldierSpawnSystem>( true );
    ::engine::Engine::Get().SetEnabled< ::engine::ctf::CtfSoldierSpawnSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::EditorSoldierSpawnSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::RoomEditorSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::EditorSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::EditorTargetSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::EditorGridSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::EditorBrushSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::core::FreeForAllGameModeSystem>( true );
    ::engine::Engine::Get().SetEnabled< ::core::CaptureTheFlagGameModeSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::core::RogueGameModeSystem>( false );

    auto levelSelectionSystem = engine::Engine::Get().GetSystem<LevelSelectionSystem>();
    if ( levelSelectionSystem.IsValid() )
    {
        mScene.Load( levelSelectionSystem->GetSelectedLevel() );
    }
    else
    {
        L1("failed to retrieve LevelSelectionSystem\n");
        return;
    }
    //    glfwSetInputMode(engine::Engine::Get().GetSystem<engine::WindowSystem>()->GetWindow(),GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
    Ui::Get().Load( "hud" );
    if ( ProgramState::Get().mMode == ProgramState::Client )
    {
        return;
    }

    if ( ProgramState::Get().mMode != ProgramState::Local )
    {
        return;
    }

    std::auto_ptr<Actor> Pl;
    for( int i = 0; i < 11; ++i )
    {
        Pl = ActorFactory::Get()( AutoId( "player" ) );

        Opt<IPositionComponent> positionC = Pl->Get<IPositionComponent>();
        positionC->SetX( -240 + i * 60 );
        positionC->SetY( 0.0 );
        Pl->Get<IInventoryComponent>();
        Opt<IInventoryComponent> inventoryC = Pl->Get<IInventoryComponent>();
        inventoryC->AddItem( AutoId( "plasma_gun" ) );
        inventoryC->SetSelectedWeapon( AutoId( "plasma_gun" ) );
        Opt<PlayerControllerComponent> pcc( Pl->Get<IControllerComponent>() );
        pcc->SetEnabled( false );
        pcc->mActive = false;
        pcc->mControllerId = i;
        Pl->Get<IMoveComponent>()->SetMoving( false );
        mScene.AddActor( Pl.release() );
    }
    EventServer<map::LevelGeneratedEvent>::Get().SendEvent( map::LevelGeneratedEvent( map::LevelGeneratedEvent::ActorsSpawned ) );
}

void FreeForAllGameModeSystem::OnLevelSelected( core::LevelSelectedEvent const& Evt )
{
    if ( GameModes::FFA != mProgramState.mGameMode )
    {
        return;
    }
    // the host did the last step in config, redirect it to the client list
    Ui::Get().Load( "ffa_client_list" );
}


} // namespace core

