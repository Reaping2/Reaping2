#include "platform/i_platform.h"
#include "network/my_name_message_handler_sub_system.h"
#include "my_name_message.h"
#include "client_id_message.h"
namespace network {

    MyNameMessageHandlerSubSystem::MyNameMessageHandlerSubSystem()
        : mMessageHolder(MessageHolder::Get())
    {

    }

    void MyNameMessageHandlerSubSystem::Init()
    {

    }

    void MyNameMessageHandlerSubSystem::Execute(Message const& message)
    {
        MyNameMessage const& msg=static_cast<MyNameMessage const&>(message);
        L1("executing myname: %s from id: %d \n",msg.mName.c_str(),msg.mSenderId );

        std::auto_ptr<ClientIdMessage> newmsg(new ClientIdMessage);
        newmsg->mName=msg.mName;
        newmsg->mClientId=msg.mSenderId;
        mMessageHolder.AddOutgoingMessage(std::auto_ptr<Message>(newmsg.release()));
    }

} // namespace engine

