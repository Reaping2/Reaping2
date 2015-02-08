#ifndef INCLUDED_NETWORK_POSITION_MESSAGE_SENDER_SYSTEM_H
#define INCLUDED_NETWORK_POSITION_MESSAGE_SENDER_SYSTEM_H

#include "message_sender_system.h"
#include "position_message.h"

namespace network {

class PositionMessageSenderSystem: public MessageSenderSystem
{
public:
    DEFINE_SYSTEM_BASE(PositionMessageSenderSystem)
    PositionMessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );

    static std::auto_ptr<PositionMessage> GeneratePositionMessage(Actor const& actor);

};

} // namespace network

#endif//INCLUDED_NETWORK_POSITION_MESSAGE_SENDER_SYSTEM_H
