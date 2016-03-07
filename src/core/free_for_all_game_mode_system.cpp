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
    if ( Evt.mMode != "ffa" )
    {
        return;
    }
    ::engine::Engine::Get().SetEnabled< ::engine::SoldierSpawnSystem>( true );
    ::engine::Engine::Get().SetEnabled< ::engine::ctf::CtfSoldierSpawnSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::EditorSoldierSpawnSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::EditorSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::core::FreeForAllGameModeSystem>( true );
    ::engine::Engine::Get().SetEnabled< ::core::CaptureTheFlagGameModeSystem>( false );

    mScene.Load( mScene.GetSelectedLevel() );
    //    glfwSetInputMode(engine::Engine::Get().GetSystem<engine::WindowSystem>()->GetWindow(),GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
    Ui::Get().Load( "hud" );
    if ( ProgramState::Get().mMode == ProgramState::Client )
    {
        return;
    }

    struct point
    {
        double x;
        double y;
    };
    const size_t NumPoints = 18;
    const point points[NumPoints] =
    {
        { -1000, -400},                                      { 0.0, -400},
        { -1000, -600}, { -800, -600}, { -600, -600},              { 0.0, -600},
        { -1000, -800},                                      { 0.0, -800},
        { -1000, -1000},                                        { 0.0, -1000},
        { -1000, -1200},                                      { 0.0, -1200}, { 200, -1200}, { 400, -1200},
        { -1000, -1400},            { -600, -1400}, { -400, -1400},  { 0.0, -1400},
    };

    //     for( size_t i = 0; i < NumPoints; ++i )
    //     {
    //         std::auto_ptr<Actor> wall = ActorFactory::Get()(AutoId("wall"));
    //         Opt<IPositionComponent> wallPositionC = wall->Get<IPositionComponent>();
    //         wallPositionC->SetX( points[i].x+600 );
    //         wallPositionC->SetY( points[i].y+400 );
    //
    //         mScene.AddActor( wall.release() );
    //     }
    //
    //     for( int32_t i = -5; i < 5; ++i )
    //     {
    //         std::auto_ptr<Actor> wall = ActorFactory::Get()(AutoId("wall"));
    //         Opt<IPositionComponent> wallPositionC = wall->Get<IPositionComponent>();
    //         wallPositionC->SetX( i*200 );
    //         wallPositionC->SetY( std::abs(i)*-100+1200 );
    //
    //         mScene.AddActor( wall.release() );
    //         wall = ActorFactory::Get()(AutoId("wall"));
    //         wallPositionC = wall->Get<IPositionComponent>();
    //         wallPositionC->SetX( (i+2)*100 );
    //         wallPositionC->SetY( (i-5)*50-1400 );
    //
    //         mScene.AddActor( wall.release() );
    //     }
    if ( ProgramState::Get().mMode != ProgramState::Local )
    {
        return;
    }
    static const size_t BenchmarkCreeps = 0;
    glm::vec4 const& dimensions = mScene.GetDimensions();
    for( size_t i = 0; i < BenchmarkCreeps; ++i )
    {
        mScene.AddTestCreep( dimensions.x + ( rand() % ( int )( ( dimensions.z - dimensions.x ) ) )
                             , dimensions.y + ( rand() % ( int )( ( dimensions.w - dimensions.y ) ) ) );

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
        Pl->Get<PlayerControllerComponent>()->SetEnabled( false );
        Pl->Get<PlayerControllerComponent>()->mActive = false;
        Pl->Get<PlayerControllerComponent>()->mControllerId = i;
        Pl->Get<IMoveComponent>()->SetMoving( false );
        mScene.AddActor( Pl.release() );
    }
}

void FreeForAllGameModeSystem::OnLevelSelected( core::LevelSelectedEvent const& Evt )
{
    if ( "ffa" != mProgramState.mGameMode )
    {
        return;
    }
    // the host did the last step in config, redirect it to the client list
    Ui::Get().Load( "ffa_client_list" );
}


} // namespace core

