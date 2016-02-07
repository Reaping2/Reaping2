#ifndef INCLUDED_NETWORK_TEAM_SWITCH_REQUEST_H
#define INCLUDED_NETWORK_TEAM_SWITCH_REQUEST_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"

namespace network {

class TeamSwitchRequestMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(TeamSwitchRequestMessage)
    TeamSwitchRequestMessage()
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void TeamSwitchRequestMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
}

class TeamSwitchRequestMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(TeamSwitchRequestMessageHandlerSubSystem)
    TeamSwitchRequestMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
    virtual void Update(double DeltaTime);
};

class TeamSwitchRequestMessageSenderSystem : public MessageSenderSystem
{
public:
    DEFINE_SYSTEM_BASE(TeamSwitchRequestMessageSenderSystem)
    TeamSwitchRequestMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace network

#endif//INCLUDED_NETWORK_TEAM_SWITCH_REQUEST_H
//command:  "./classgenerator" -g "message" -c "team_switch_request"
