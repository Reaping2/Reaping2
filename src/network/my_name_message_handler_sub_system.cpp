#include "platform/i_platform.h"
#include "network/my_name_message_handler_sub_system.h"
#include "my_name_message.h"
#include "client_id_message.h"
#include "client_system.h"
#include "core/program_state.h"
#include "engine/client_datas_changed_event.h"
#include "lifecycle_message.h"
#include "core/ctf_program_state.h"
#include "ctf_client_datas_changed_event.h"
#include "client_datas_message.h"
#include "actor_list_message.h"
#include "portable_oarchive.hpp"
#include "core/scene.h"
#include "set_ownership_message.h"
#include "client_id_changed.h"
#include "ctf_client_datas_message.h"
#include "core/ctf_program_state.h"
#include "client_list_changed_event.h"
#include "engine/connection_event.h"
#include "data_checksum_message.h"
#include "platform/checksum.h"


namespace network {

MyNameMessageHandlerSubSystem::MyNameMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{

}

void MyNameMessageHandlerSubSystem::Init()
{

}

// Server gets the name from the client. Then server decides in what state the client is.
void MyNameMessageHandlerSubSystem::Execute( Message const& message )
{
    MyNameMessage const& msg = static_cast<MyNameMessage const&>( message );
    L1( "executing myname: %s from current id: %d \n", msg.mName.c_str(), msg.mSenderId );



    Opt<core::ClientData> clientData( core::ProgramState::Get().FindClientDataByClientName( msg.mName ) );
    if ( clientData.IsValid() )
    {
        if ( clientData->mConnected )
        {
            L1( "client wth the name: %s is already connected.\n", msg.mName.c_str() );
            std::auto_ptr<LifecycleMessage> lifecycleMsg( new LifecycleMessage );
            lifecycleMsg->mState = LifecycleMessage::AlreadyConnected;
            lifecycleMsg->mClientId = msg.mSenderId;
            mMessageHolder.AddOutgoingMessage( std::auto_ptr<Message>( lifecycleMsg.release() ) );
            return;
        }
        clientData->mConnected = true;
        EventServer<ClientIdChangedEvent>::Get().SendEvent( ClientIdChangedEvent( msg.mSenderId, clientData->mClientId ) );
        std::auto_ptr<ClientIdMessage> newmsg( new ClientIdMessage );
        newmsg->mName = msg.mName;
        newmsg->mClientId = clientData->mClientId;
        mMessageHolder.AddOutgoingMessage( std::auto_ptr<Message>( newmsg.release() ) );
        L1( "client was connected before setting setting clientId back to old clientId! name: %s old id: %d new id: %d \n", msg.mName.c_str(), msg.mSenderId, clientData->mClientId );
        EventServer<engine::ConnectionEvent>::Get().SendEvent( engine::ConnectionEvent( clientData->mClientId, engine::ConnectionEvent::Reconnected ) );

        if ( clientData->mSoldierProperties.mArrived )
        {
            if ( mProgramState.mGameState == core::ProgramState::Running )
            {
                L1( "client sent soldier prorties before! Game is running. Sending everything to name: %s\n", msg.mName.c_str() );
                std::auto_ptr<ClientDatasMessage> clientDatasMessage( new ClientDatasMessage );
                clientDatasMessage->mClientDatas = mProgramState.mClientDatas;
                mMessageHolder.AddOutgoingMessage( std::auto_ptr<Message>( clientDatasMessage.release() ) );

                std::auto_ptr<ctf::ClientDatasMessage> message( new ctf::ClientDatasMessage );
                message->mClientDatas = ::ctf::ProgramState::Get().mClientDatas;
                mMessageHolder.AddOutgoingMessage( message );

                std::auto_ptr<LifecycleMessage> lifecycleMsg( new LifecycleMessage );
                lifecycleMsg->mState = LifecycleMessage::Start;
                lifecycleMsg->mGameMode = mProgramState.mGameMode;
                lifecycleMsg->mClientId = clientData->mClientId;
                mMessageHolder.AddOutgoingMessage( std::auto_ptr<Message>( lifecycleMsg.release() ) );

                std::auto_ptr<ActorListMessage> actorListMsg( new ActorListMessage );
                actorListMsg->mClientId = clientData->mClientId;
                actorListMsg->mActorList = &Scene::Get().GetActors();
                mMessageHolder.AddOutgoingMessage( actorListMsg );

                std::auto_ptr<SetOwnershipMessage> setOwnershipMsg( new SetOwnershipMessage );
                setOwnershipMsg->mActorGUID = clientData->mClientActorGUID;
                setOwnershipMsg->mClientId = clientData->mClientId;
                mMessageHolder.AddOutgoingMessage( setOwnershipMsg );
            }
            else
            {
                L1( "client sent soldier prorties before! setting client to waitingforhost. name: %s\n", msg.mName.c_str() );
                std::auto_ptr<LifecycleMessage> lifecycleMsg( new LifecycleMessage );
                lifecycleMsg->mState = LifecycleMessage::WaitingForHost;
                lifecycleMsg->mClientId = clientData->mClientId;
                mMessageHolder.AddOutgoingMessage( std::auto_ptr<Message>( lifecycleMsg.release() ) );

                std::auto_ptr<ClientDatasMessage> clientDatasMessage( new ClientDatasMessage );
                clientDatasMessage->mClientDatas = mProgramState.mClientDatas;
                mMessageHolder.AddOutgoingMessage( std::auto_ptr<Message>( clientDatasMessage.release() ) );
            }
        }
        else
        {
            L1( "client never sent soldier properties! setting client to soldier_properties. name: %s\n", msg.mName.c_str() );
            std::auto_ptr<LifecycleMessage> lifecycleMsg( new LifecycleMessage );
            lifecycleMsg->mState = LifecycleMessage::SoldierProperties;
            lifecycleMsg->mClientId = clientData->mClientId;
            mMessageHolder.AddOutgoingMessage( std::auto_ptr<Message>( lifecycleMsg.release() ) );
        }
    }
    else
    {
        L1( "absolutely new client is connected! name %s clientId: %d \n", msg.mName.c_str(), msg.mSenderId );
        std::auto_ptr<ClientIdMessage> newmsg( new ClientIdMessage );
        newmsg->mName = msg.mName;
        newmsg->mClientId = msg.mSenderId;
        mMessageHolder.AddOutgoingMessage( std::auto_ptr<Message>( newmsg.release() ) );

        mProgramState.mClientDatas.push_back( core::ClientData( msg.mSenderId, msg.mName ) );

        std::auto_ptr<LifecycleMessage> lifecycleMsg( new LifecycleMessage );
        lifecycleMsg->mState = LifecycleMessage::SoldierProperties;
        lifecycleMsg->mClientId = msg.mSenderId;
        mMessageHolder.AddOutgoingMessage( std::auto_ptr<Message>( lifecycleMsg.release() ) );

        EventServer<engine::ConnectionEvent>::Get().SendEvent( engine::ConnectionEvent( msg.mSenderId, engine::ConnectionEvent::Connected ) );
    }
    // calculate checksums and distribute it to the clients
    {
        // TODO: not cached, NOT COOL!
        // case 1: it is data.pkg
        Package pkg( AutoFile( new OsFile("data.pkg") ) );

        static boost::uint32_t datapkgChecksum = pkg.Checksum();
        static boost::uint32_t autoidChecksum = fileChecksum("autoids");
        int32_t clientId = clientData.IsValid() ? clientData->mClientId : msg.mSenderId;

        std::auto_ptr<DataChecksumMessage> autoidChecksumMsg( new DataChecksumMessage );
        autoidChecksumMsg->mDatasource = "autoids";
        autoidChecksumMsg->mChecksum = autoidChecksum;
        autoidChecksumMsg->mClientId = clientId;
        mMessageHolder.AddOutgoingMessage( std::auto_ptr<DataChecksumMessage>(autoidChecksumMsg.release() ) );

        std::auto_ptr<DataChecksumMessage> datapkgChecksumMsg( new DataChecksumMessage );
        datapkgChecksumMsg->mDatasource = "data.pkg";
        datapkgChecksumMsg->mChecksum = datapkgChecksum;
        datapkgChecksumMsg->mClientId = clientId;
        mMessageHolder.AddOutgoingMessage( std::auto_ptr<DataChecksumMessage>(datapkgChecksumMsg.release() ) );
    }
    EventServer<engine::ClientDatasChangedEvent>::Get().SendEvent( engine::ClientDatasChangedEvent() );
}

} // namespace engine

