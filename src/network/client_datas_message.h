#ifndef INCLUDED_NETWORK_CLIENT_DATAS_H
#define INCLUDED_NETWORK_CLIENT_DATAS_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "single_message_sender.h"
#include "platform/export.h"

namespace network {

class ClientDatasMessage: public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE( ClientDatasMessage )
    core::ProgramState::ClientDatas_t mClientDatas;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ClientDatasMessage::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Message>( *this );
    ar& mClientDatas;
}

class ClientDatasMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( ClientDatasMessageHandlerSubSystem )
    ClientDatasMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute( Message const& message );
};

class ClientDatasMessageSenderSystem : public MessageSenderSystem
{
public:
    DEFINE_SYSTEM_BASE( ClientDatasMessageSenderSystem )
    ClientDatasMessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
};
} // namespace network

REAPING2_CLASS_EXPORT_KEY2( network__ClientDatasMessage, network::ClientDatasMessage, "client_datas" );
#endif//INCLUDED_NETWORK_CLIENT_DATAS_H

// TODO: to main.cpp:
// Eng.AddSystem(AutoId("client_datas_message_sender_system"));


