#ifndef INCLUDED_NETWORK_MOVE_MESSAGE_SENDER_SYSTEM_H
#define INCLUDED_NETWORK_MOVE_MESSAGE_SENDER_SYSTEM_H

#include "message_sender_system.h"

namespace network {

class MoveMessageSenderSystem: public MessageSenderSystem
{
public:
    DEFINE_SYSTEM_BASE(MoveMessageSenderSystem)
    MoveMessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
};

} // namespace network

#endif//INCLUDED_NETWORK_MOVE_MESSAGE_SENDER_SYSTEM_H
