#include "platform/i_platform.h"
#include "capture_the_flag_game_mode_system.h"
#include "ui/ui.h"
#include "core/ctf_program_state.h"
#include "network/ctf_client_datas_message.h"
#include "network/messsage_holder.h"
#include "engine/soldier_spawn_system.h"
#include "engine/engine.h"
#include "engine/ctf_soldier_spawn_system.h"
#include "i_team_component.h"
#include "platform/event.h"
#include "engine/show_text_event.h"
#include "map/editor_system.h"
#include "map/editor_soldier_spawn_system.h"
#include "engine/client_score_event.h"
#include "free_for_all_game_mode_system.h"
#include "rogue_game_mode_system.h"
#include "map/editor_brush_system.h"
#include "map/editor_grid_system.h"
#include "map/editor_target_system.h"
#include "map/room_editor_system.h"
#include "level_selection_system.h"

namespace core {

CaptureTheFlagGameModeSystem::CaptureTheFlagGameModeSystem()
    : mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
    , mCtfProgramState( ctf::ProgramState::Get() )
    , mCtfModel( "ctf", &RootModel::Get() )
    , mHudShown( false )
{
}


void CaptureTheFlagGameModeSystem::Init()
{
    mOnStartGameMode = EventServer<core::StartGameModeEvent>::Get().Subscribe( boost::bind( &CaptureTheFlagGameModeSystem::OnStartGameMode, this, _1 ) );
    mOnFlagStateChanged = EventServer<ctf::FlagStateChangedEvent>::Get().Subscribe( boost::bind( &CaptureTheFlagGameModeSystem::OnFlagStateChanged, this, _1 ) );
    mOnScore = EventServer<engine::ScoreEvent>::Get().Subscribe( boost::bind( &CaptureTheFlagGameModeSystem::OnScore, this, _1 ) );
    mOnLevelSelected = EventServer<core::LevelSelectedEvent>::Get().Subscribe( boost::bind( &CaptureTheFlagGameModeSystem::OnLevelSelected, this, _1 ) );
    mInputSystem = engine::InputSystem::Get();
    mTeamModels.clear();
    mTeamModels.push_back( new ModelValue( GetIntFunc( &mCtfProgramState, &ctf::ProgramState::GetBlueScore ), "blue", &mCtfModel ) );
    mTeamModels.push_back( new ModelValue( GetIntFunc( &mCtfProgramState, &ctf::ProgramState::GetRedScore ), "red", &mCtfModel ) );
}


void CaptureTheFlagGameModeSystem::Update( double DeltaTime )
{
    if ( mProgramState.mMode == core::ProgramState::Server )
    {
        if ( mCtfProgramState.mBlueScore >= 40 )
        {
            EventServer<engine::ShowTextEvent>::Get().SendEvent( engine::ShowTextEvent( 40000, "BLUE TEAM IS VICTORIOUS!" ) );
        }
        if ( mCtfProgramState.mRedScore >= 40 )
        {
            EventServer<engine::ShowTextEvent>::Get().SendEvent( engine::ShowTextEvent( 40000, "RED TEAM IS VICTORIOUS!" ) );
        }
    }
    if ( mInputSystem.IsValid() )
    {
        if ( mHudShown && mInputSystem->GetInputState( 1 ).mShowLeaderboard )
        {
            mHudShown = false;
            Ui::Get().Load( "leaderboard" );
        }
        else if ( !mHudShown && !mInputSystem->GetInputState( 1 ).mShowLeaderboard )
        {
            mHudShown = true;
            Ui::Get().Load( "ctf_hud" );
        }
    }
}

void CaptureTheFlagGameModeSystem::OnStartGameMode( core::StartGameModeEvent const& Evt )
{
    if ( Evt.mMode != "ctf" )
    {
        return;
    }
    mCtfProgramState.ResetScores();

    ::engine::Engine::Get().SetEnabled< ::engine::SoldierSpawnSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::engine::ctf::CtfSoldierSpawnSystem>( true );
    ::engine::Engine::Get().SetEnabled< ::map::EditorSoldierSpawnSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::RoomEditorSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::EditorSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::EditorTargetSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::EditorGridSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::EditorBrushSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::core::FreeForAllGameModeSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::core::CaptureTheFlagGameModeSystem>( true );
    ::engine::Engine::Get().SetEnabled< ::core::RogueGameModeSystem>( false );


    if ( mProgramState.mMode == core::ProgramState::Server )
    {
        EventServer<engine::ShowTextEvent>::Get().SendEvent( engine::ShowTextEvent( 4, "Welcome to HELL Bitches!" ) );
        EventServer<engine::ShowTextEvent>::Get().SendEvent( engine::ShowTextEvent( 4, "Capture that flag" ) );
        EventServer<engine::ShowTextEvent>::Get().SendEvent( engine::ShowTextEvent( 4, "GL HF!" ) );
        ctf::ProgramState& ctfProgramState = ctf::ProgramState::Get();
        std::auto_ptr<network::ctf::ClientDatasMessage> clientDatasMsg( new network::ctf::ClientDatasMessage );
        clientDatasMsg->mClientDatas = ctfProgramState.mClientDatas;
        network::MessageHolder::Get().AddOutgoingMessage( clientDatasMsg );
    }
    mCtfProgramState.mRedScore = 0;
    mCtfProgramState.mBlueScore = 0;
    auto levelSelectionSystem = engine::Engine::Get().GetSystem<LevelSelectionSystem>();
    if ( levelSelectionSystem.IsValid() )
    {
        mScene.Load( levelSelectionSystem->GetSelectedLevel() );
        Ui::Get().Load( "ctf_hud" );
        mHudShown = true;
    }
    else
    {
        L1("failed to retrieve LevelSelectionSystem\n");
    }
}

void CaptureTheFlagGameModeSystem::OnFlagStateChanged( ctf::FlagStateChangedEvent const& Evt )
{
    Opt<core::ClientData> clientData( mProgramState.FindClientDataByActorGUID( Evt.mCarrierGUID ) );
    std::string name = "Someone";
    static const double messageTime = 2.5;
    if ( clientData.IsValid() )
    {
        name = clientData->mClientName;
    }
    if ( Evt.mType == ctf::FlagStateChangedEvent::Scored )
    {
        if ( clientData.IsValid() )
        {
            clientData->mScore += 5;
            EventServer<engine::ClientScoreEvent>::Get().SendEvent( engine::ClientScoreEvent( clientData->mClientId, clientData->mScore ) );
        }
        if( Evt.mTeam == Team::Blue )
        {
            mCtfProgramState.mRedScore += 5;
            EventServer<engine::ShowTextEvent>::Get().SendEvent( engine::ShowTextEvent( messageTime, name + " scored for Red team!" ) );
        }
        else
        {
            mCtfProgramState.mBlueScore += 5;
            EventServer<engine::ShowTextEvent>::Get().SendEvent( engine::ShowTextEvent( messageTime, name + " scored for Blue team!" ) );
        }
    }
    else if ( Evt.mType == ctf::FlagStateChangedEvent::Captured )
    {
        if( Evt.mTeam == Team::Blue )
        {
            EventServer<engine::ShowTextEvent>::Get().SendEvent( engine::ShowTextEvent( messageTime, name + " has the Blue flag!!" ) );
        }
        else
        {
            EventServer<engine::ShowTextEvent>::Get().SendEvent( engine::ShowTextEvent( messageTime, name + " has the Red flag!" ) );
        }
    }
    else if ( Evt.mType == ctf::FlagStateChangedEvent::Returned )
    {
        if( Evt.mTeam == Team::Blue )
        {
            EventServer<engine::ShowTextEvent>::Get().SendEvent( engine::ShowTextEvent( messageTime, name + " returned the Blue flag!" ) );
        }
        else
        {
            EventServer<engine::ShowTextEvent>::Get().SendEvent( engine::ShowTextEvent( messageTime, name + " returned the Red flag!" ) );
        }
    }
    else if ( Evt.mType == ctf::FlagStateChangedEvent::Dropped )
    {
        if( Evt.mTeam == Team::Blue )
        {
            EventServer<engine::ShowTextEvent>::Get().SendEvent( engine::ShowTextEvent( messageTime, name + " has dropped the Blue flag!" ) );
        }
        else
        {
            EventServer<engine::ShowTextEvent>::Get().SendEvent( engine::ShowTextEvent( messageTime, name + " has dropped the Red flag!" ) );
        }
    }
}

void CaptureTheFlagGameModeSystem::OnScore( engine::ScoreEvent const& Evt )
{
    Opt<Actor> actor = mScene.GetActor( Evt.mActorGUID );
    Opt<ITeamComponent> actorTeamC( actor->Get<ITeamComponent>() );
    if ( !actorTeamC.IsValid() )
    {
        return;
    }
    if ( actorTeamC->GetTeam() == Team::Blue )
    {
        mCtfProgramState.mRedScore += 1;
    }
    else
    {
        mCtfProgramState.mBlueScore += 1;
    }
}

void CaptureTheFlagGameModeSystem::OnLevelSelected( core::LevelSelectedEvent const& Evt )
{
    if ( "ctf" != mProgramState.mGameMode )
    {
        return;
    }
    // the host did the last step in config, redirect it to the client list
    Ui::Get().Load( "ctf_client_list" );
}

} // namespace core

