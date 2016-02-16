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
namespace network {

    MyNameMessageHandlerSubSystem::MyNameMessageHandlerSubSystem()
        : MessageHandlerSubSystem()
    {

    }

    void MyNameMessageHandlerSubSystem::Init()
    {

    }

    // Server gets the name from the client. Then server decides in what state the client is.
    void MyNameMessageHandlerSubSystem::Execute(Message const& message)
    {
        MyNameMessage const& msg=static_cast<MyNameMessage const&>(message);
        L1("executing myname: %s from id: %d \n",msg.mName.c_str(),msg.mSenderId );

        std::auto_ptr<ClientIdMessage> newmsg(new ClientIdMessage);
        newmsg->mName=msg.mName;
        newmsg->mClientId=msg.mSenderId;
        mMessageHolder.AddOutgoingMessage(std::auto_ptr<Message>(newmsg.release()));

        Opt<core::ClientData> clientData(core::ProgramState::Get().FindClientDataByClientName(msg.mName));
        if (clientData.IsValid())
        {
            if (clientData->mConnected)
            {
                L1("client wth the name: %s is already connected.\n",msg.mName.c_str());
                std::auto_ptr<LifecycleMessage> lifecycleMsg(new LifecycleMessage);
                lifecycleMsg->mState=LifecycleMessage::AlreadyConnected;
                lifecycleMsg->mClientId=msg.mSenderId;
                mMessageHolder.AddOutgoingMessage(std::auto_ptr<Message>(lifecycleMsg.release()));
                return;
            }
            clientData->mConnected=true;
            clientData->mClientId=msg.mSenderId;
            //TODO: event on client id change
            Opt<ctf::ClientData> ctfClientData(ctf::ProgramState::Get().FindClientDataByClientId(msg.mSenderId));
            if (ctfClientData.IsValid())
            {
                ctfClientData->mClientId=msg.mSenderId;
            }
            L1("client was connected before setting new clientId to old data! name: %s from id: %d \n",msg.mName.c_str(),msg.mSenderId );

            std::auto_ptr<LifecycleMessage> lifecycleMsg(new LifecycleMessage);
            if (clientData->mSoldierProperties.mArrived)
            {
                L1("client sent soldier prorties before! setting client to waitingforhost. name: %s\n",msg.mName.c_str() );
                lifecycleMsg->mState=LifecycleMessage::WaitingForHost;
                std::auto_ptr<ClientDatasMessage> clientDatasMessage( new ClientDatasMessage );
                clientDatasMessage->mClientDatas = mProgramState.mClientDatas;
                mMessageHolder.AddOutgoingMessage(std::auto_ptr<Message>(clientDatasMessage.release()));
            }
            else
            {
                L1("client never sent soldier properties! setting client to soldier_properties. name: %s\n",msg.mName.c_str() );
                lifecycleMsg->mState=LifecycleMessage::SoldierProperties;
            }
            lifecycleMsg->mClientId=clientData->mClientId;
            mMessageHolder.AddOutgoingMessage(std::auto_ptr<Message>(lifecycleMsg.release()));
        }
        else
        {
            mProgramState.mClientDatas.push_back(core::ClientData(msg.mSenderId,msg.mName));

            std::auto_ptr<LifecycleMessage> lifecycleMsg(new LifecycleMessage);
            lifecycleMsg->mState=LifecycleMessage::SoldierProperties;
            lifecycleMsg->mClientId=msg.mSenderId;
            mMessageHolder.AddOutgoingMessage(std::auto_ptr<Message>(lifecycleMsg.release()));
        }
        EventServer<engine::ClientDatasChangedEvent>::Get().SendEvent(engine::ClientDatasChangedEvent());
    }

} // namespace engine

