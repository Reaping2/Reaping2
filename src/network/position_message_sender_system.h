#ifndef INCLUDED_NETWORK_POSITION_MESSAGE_SENDER_SYSTEM_H
#define INCLUDED_NETWORK_POSITION_MESSAGE_SENDER_SYSTEM_H

#include "message_sender_system.h"
#include "core/scene.h"

namespace network {

class PositionMessageSenderSystem: public MessageSenderSystem
{
public:
    DEFINE_SYSTEM_BASE(PositionMessageSenderSystem)
    PositionMessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
};

} // namespace network

#endif//INCLUDED_NETWORK_POSITION_MESSAGE_SENDER_SYSTEM_H
