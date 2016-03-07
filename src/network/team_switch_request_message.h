#ifndef INCLUDED_NETWORK_TEAM_SWITCH_REQUEST_H
#define INCLUDED_NETWORK_TEAM_SWITCH_REQUEST_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "network/team_switch_request_event.h"
#include "platform/export.h"

namespace network {

class TeamSwitchRequestMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    int32_t mClientId;
    DEFINE_MESSAGE_BASE( TeamSwitchRequestMessage )
    TeamSwitchRequestMessage()
    {
    }
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void TeamSwitchRequestMessage::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Message>( *this );
    ar& mClientId;
}

class TeamSwitchRequestMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( TeamSwitchRequestMessageHandlerSubSystem )
    TeamSwitchRequestMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute( Message const& message );
    virtual void Update( double DeltaTime );
};

class TeamSwitchRequestMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnTeamSwitchRequestEvent;
    void OnTeamSwitchRequest( TeamSwitchRequestEvent const& event );
public:
    DEFINE_SYSTEM_BASE( TeamSwitchRequestMessageSenderSystem )
    TeamSwitchRequestMessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
};
} // namespace network

REAPING2_CLASS_EXPORT_KEY2( network__TeamSwitchRequestMessage, network::TeamSwitchRequestMessage, "team_switch_request_message" )
#endif//INCLUDED_NETWORK_TEAM_SWITCH_REQUEST_H
//command:  "./classgenerator" -g "message" -c "team_switch_request"
