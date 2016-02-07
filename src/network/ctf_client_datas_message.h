#ifndef INCLUDED_NETWORK_CTF_CLIENT_DATAS_H
#define INCLUDED_NETWORK_CTF_CLIENT_DATAS_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "core/ctf_program_state.h"
#include "network/ctf_client_datas_changed_event.h"

namespace network {
namespace ctf {

class ClientDatasMessage: public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(::network::ctf::ClientDatasMessage)
    ::ctf::ProgramState::ClientDatas_t mClientDatas; 
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void ClientDatasMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
    ar & mClientDatas;
}

class ClientDatasMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(::network::ctf::ClientDatasMessageHandlerSubSystem)
    ClientDatasMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
};

class ClientDatasMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnCtfClientDatasChanged;
    void OnCtfClientDatasChangedEvent( CtfClientDatasChangedEvent const & evt );
public:
    DEFINE_SYSTEM_BASE(::network::ctf::ClientDatasMessageSenderSystem)
    ClientDatasMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace ctf
} // namespace network

#endif//INCLUDED_NETWORK_CTF_CLIENT_DATAS_H

// TODO: to main.cpp:
// Eng.AddSystem(AutoId("client_datas_message_sender_system"));


