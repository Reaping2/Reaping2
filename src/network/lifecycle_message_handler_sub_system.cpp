#include "platform/i_platform.h"
#include "lifecycle_message_handler_sub_system.h"
#include "core/program_state.h"
#include "network/lifecycle_message.h"
#include "core/scene.h"
#include "ui/ui.h"
#include "messsage_holder.h"
#include "create_actor_message.h"
#include "core/actor_factory.h"
#include "core/i_position_component.h"
#include "set_ownership_message.h"
#include "position_message.h"
#include "core/program_state.h"
#include "core/i_inventory_component.h"
#include "core/i_move_component.h"
#include "core/buffs/i_buff_holder_component.h"
#include "core/buffs/buff_factory.h"
#include "core/buffs/move_speed_buff.h"
#include "client_datas_message.h"
#include "engine/soldier_spawn_system.h"
#include "core/start_game_mode_event.h"
#include "engine/engine.h"
#include "main/window.h"
#include "load_clientlist_event.h"
#include "core/level_selection_system.h"
#include "actor_list_message.h"

namespace network {

LifecycleMessageHandlerSubSystem::LifecycleMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{

}

void LifecycleMessageHandlerSubSystem::Init()
{
    mOnMapStart = EventServer<core::MapStartEvent>::Get().Subscribe( boost::bind( &LifecycleMessageHandlerSubSystem::OnMapStart, this, _1 ) );
}

void LifecycleMessageHandlerSubSystem::Execute( Message const& message )
{
    LifecycleMessage const& msg = static_cast<LifecycleMessage const&>( message );
    L2( "executing lifecycle: state: %d \n", msg.mState );
    if ( mProgramState.mMode == ProgramState::Client
         && ( msg.mClientId == -1 || msg.mClientId == mProgramState.mClientId ) )
    {
        if ( msg.mState == LifecycleMessage::Start )
        {
            mProgramState.mGameState = core::ProgramState::Running;
            mProgramState.mGameMode = msg.mGameMode;
        }
        else if ( msg.mState == LifecycleMessage::WaitingForHost )
        {
            Ui::Get().Load( "waiting_start" );
        }
        else if ( msg.mState == LifecycleMessage::SoldierProperties )
        {
            Ui::Get().Load( "soldier_properties" );
        }
        else if ( msg.mState == LifecycleMessage::ClientList )
        {
            Opt<core::ClientData> clientData = mProgramState.FindClientDataByClientId( mProgramState.mClientId );
            if ( clientData.IsValid() && clientData->mSoldierProperties.mArrived )
            {
                network::LoadClientlistEvent event;
                event.mGameMode = msg.mGameMode;
                EventServer<network::LoadClientlistEvent>::Get().SendEvent( event );
            }
        }
        else if ( msg.mState == LifecycleMessage::SelectLevel )
        {
            Ui::Get().Load( "select_level" );
        }
    }
    else if ( mProgramState.mMode == ProgramState::Client
              && mProgramState.mClientId == -1 )
    {
        if ( msg.mState == LifecycleMessage::AlreadyConnected )
        {
            L1( "\n\n\n\nAlready connected. If you lost connection please try reconnecting later! *** One Love!\n" );
            engine::Engine::Get().GetSystem<engine::WindowSystem>()->Close();
        }
        else if ( msg.mState == LifecycleMessage::Start )
        {
            core::ProgramState::Get().mGameState = core::ProgramState::Running;
        }
    }
    else if ( mProgramState.mMode == ProgramState::Server )
    {
        if ( msg.mState == LifecycleMessage::Start )
        {
            auto levelSelectionSystem = engine::Engine::Get().GetSystem<core::LevelSelectionSystem>();
            if ( levelSelectionSystem.IsValid() )
            {
                levelSelectionSystem->SelectLevelByName(msg.mGameMode, msg.mSelectedLevel);
            }
            else
            {
                L1("failed to retrieve LevelSelectionSystem\n");
                return;
            }
            EventServer<core::StartGameModeEvent>::Get().SendEvent( core::StartGameModeEvent( msg.mGameMode ) );
            std::auto_ptr<network::LifecycleMessage> lifecycleMsg( new network::LifecycleMessage );
            lifecycleMsg->mState = msg.mState;
            lifecycleMsg->mGameMode = msg.mGameMode;
            mMessageHolder.AddOutgoingMessage( lifecycleMsg );
            std::auto_ptr<network::ClientDatasMessage> clientDatasMsg( new network::ClientDatasMessage );
            clientDatasMsg->mClientDatas = mProgramState.mClientDatas;
            mMessageHolder.AddOutgoingMessage( clientDatasMsg );
        }
    }
}


void LifecycleMessageHandlerSubSystem::OnMapStart( core::MapStartEvent const& Evt )
{
    if (Evt.mState == core::MapStartEvent::ActorsSpawned&&mProgramState.mMode == ProgramState::Server)
    {
        std::auto_ptr<ActorListMessage> actorListMsg( new ActorListMessage( &Scene::Get().GetActors() ) );
        actorListMsg->mClientId = -1;
        mMessageHolder.AddOutgoingMessage( actorListMsg );
        L1( "Lifecycle ActorList Sent! %d\n",Scene::Get().GetActors().size() );
    }
}

} // namespace engine

