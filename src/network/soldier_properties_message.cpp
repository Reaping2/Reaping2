#include "platform/i_platform.h"
#include "network/soldier_properties_message.h"
#include "core/buffs/i_buff_holder_component.h"
#include "core/buffs/buff_factory.h"
#include "core/buffs/move_speed_buff.h"
#include "core/buffs/max_health_buff.h"
#include "engine/buffs_engine/max_health_buff_sub_system.h"
#include "core/i_health_component.h"
#include "health_message.h"
#include "core/buffs/accuracy_buff.h"
#include "core/i_accuracy_component.h"
#include "engine/buffs_engine/accuracy_buff_sub_system.h"
#include "accuracy_message.h"
#include "network/client_datas_message.h"
#include "network/client_ready_event.h"
#include "lifecycle_message.h"
#include "ctf_client_datas_message.h"
#include "core/ctf_program_state.h"
#include "actor_list_message.h"
#include "client_list_changed_event.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
#include "waypoints_data_message.h"

namespace network {

SoldierPropertiesMessageSenderSystem::SoldierPropertiesMessageSenderSystem()
    : MessageSenderSystem()
{
    mOnSoldierCreatedEvent = EventServer<engine::SoldierCreatedEvent>::Get().Subscribe( boost::bind( &SoldierPropertiesMessageSenderSystem::OnSoldierCreatedEvent, this, _1 ) );
    mOnSoldierPropertiesReady = EventServer<engine::SoldierPropertiesReadyEvent>::Get().Subscribe( boost::bind( &SoldierPropertiesMessageSenderSystem::OnSoldierPropertiesReady, this, _1 ) );

}

void SoldierPropertiesMessageSenderSystem::OnSoldierCreatedEvent( engine::SoldierCreatedEvent const& Evt )
{
    if (Evt.mState != engine::SoldierCreatedEvent::Finished)
    {
        return;
    }
    if( mProgramState.mMode == ProgramState::Server )
    {
        Opt<IBuffHolderComponent> buffHolderC = Evt.mActor->Get<IBuffHolderComponent>();
        if( buffHolderC.IsValid() )
        {
            mMessageHolder.AddOutgoingMessage( network::HealthMessageSenderSystem::GenerateHealthMessage( *Evt.mActor ) );
            mMessageHolder.AddOutgoingMessage( network::AccuracyMessageSenderSystem::GenerateAccuracyMessage( *Evt.mActor ) );
        }

    }
}

void SoldierPropertiesMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
}


void SoldierPropertiesMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
}

void SoldierPropertiesMessageSenderSystem::OnSoldierPropertiesReady( engine::SoldierPropertiesReadyEvent const& Evt )
{
    if ( mProgramState.mMode != ProgramState::Server )
    {
        std::auto_ptr<SoldierPropertiesMessage> soldierPorpertiesMessage( new SoldierPropertiesMessage );
        soldierPorpertiesMessage->mSoldierProperties = mProgramState.mSoldierProperties;
        mMessageHolder.AddOutgoingMessage( soldierPorpertiesMessage );
    }
}

SoldierPropertiesMessageHandlerSubSystem::SoldierPropertiesMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void SoldierPropertiesMessageHandlerSubSystem::Init()
{
}

void SoldierPropertiesMessageHandlerSubSystem::Execute( Message const& message )
{
    SoldierPropertiesMessage const& msg = static_cast<SoldierPropertiesMessage const&>( message );
    L2( "executing soldierProperties from id: %d \n", msg.mSenderId );

    Opt<core::ClientData> clientData( mProgramState.FindClientDataByClientId( msg.mSenderId ) );
    if ( !clientData.IsValid() )
    {
        L1( "cannot find clientdata for for properties: senderId: %d\n", msg.mSenderId );
        return;
    }

    clientData->mSoldierProperties = msg.mSoldierProperties;
    clientData->mSoldierProperties.mArrived = true;
    clientData->mReady = true;
    L1( "**** Client: %s properties arrived. Ready to fight!!! **** from id: %d \n", clientData->mClientName.c_str(), msg.mSenderId );
    L1( "   MoveSpeed:%d\n   Health:%d\n   Accuracy:%d\n", msg.mSoldierProperties.mMoveSpeed, msg.mSoldierProperties.mHealth, msg.mSoldierProperties.mAccuracy );
    // put client name here into client_list
    if ( mProgramState.mMode == ProgramState::Server )
    {
        ClientReadyEvent event;
        event.mClientId = clientData->mClientId;
        event.mClientName = clientData->mClientName;
        // for ctf
        EventServer<ClientReadyEvent>::Get().SendEvent( event );
        // for ffa
        std::auto_ptr<ClientDatasMessage> clientDatasMessage( new ClientDatasMessage );
        clientDatasMessage->mClientDatas = mProgramState.mClientDatas;
        mMessageHolder.AddOutgoingMessage( std::auto_ptr<Message>( clientDatasMessage.release() ) );

        if ( mProgramState.mGameState == core::ProgramState::Running )
        {
            std::auto_ptr<LifecycleMessage> lifecycleMsg( new LifecycleMessage );
            lifecycleMsg->mState = LifecycleMessage::Start;
            lifecycleMsg->mGameMode = mProgramState.mGameMode;
            lifecycleMsg->mClientId = clientData->mClientId;
            mMessageHolder.AddOutgoingMessage( std::auto_ptr<Message>( lifecycleMsg.release() ) );

            std::auto_ptr<ActorListMessage> actorListMsg( new ActorListMessage( &Scene::Get().GetActors() ) );
            actorListMsg->mClientId = clientData->mClientId;
            mMessageHolder.AddOutgoingMessage( actorListMsg );

            static auto waypointS( engine::Engine::Get().GetSystem<engine::WaypointSystem>() );
            if (waypointS.IsValid())
            {
                mMessageHolder.AddOutgoingMessage( std::auto_ptr<WaypointsDataMessage>(new WaypointsDataMessage( &waypointS->GetWaypointsData(), clientData->mClientId ) ) );
            }
        }
        else
        {
            if ( mProgramState.mGameMode == core::GameModes::Unknown )
            {
                std::auto_ptr<LifecycleMessage> lifecycleMsg( new LifecycleMessage );
                lifecycleMsg->mClientId = msg.mSenderId;
                lifecycleMsg->mState = LifecycleMessage::WaitingForHost;
                mMessageHolder.AddOutgoingMessage( std::auto_ptr<Message>( lifecycleMsg.release() ) );
            }
            else
            {
                std::auto_ptr<LifecycleMessage> lifecycleMsg( new LifecycleMessage );
                lifecycleMsg->mState = LifecycleMessage::ClientList;
                lifecycleMsg->mGameMode = mProgramState.mGameMode;
                lifecycleMsg->mClientId = msg.mSenderId;
                mMessageHolder.AddOutgoingMessage( std::auto_ptr<Message>( lifecycleMsg.release() ) );
            }
        }
    }
}


} // namespace network


REAPING2_CLASS_EXPORT_IMPLEMENT( network__SoldierPropertiesMessage, network::SoldierPropertiesMessage );
