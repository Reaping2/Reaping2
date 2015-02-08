#ifndef INCLUDED_NETWORK_CREATE_ACTOR_MESSAGE_SENDER_SYSTEM_H
#define INCLUDED_NETWORK_CREATE_ACTOR_MESSAGE_SENDER_SYSTEM_H

#include "message_sender_system.h"
#include "platform/register.h"
#include "core/actor_event.h"

namespace network {

class CreateActorMessageSenderSystem: public MessageSenderSystem
{
    AutoReg mOnActorEvent;
    void OnActorEvent( ActorEvent const& Evt );
public:
    DEFINE_SYSTEM_BASE(CreateActorMessageSenderSystem)
    CreateActorMessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
};

} // namespace network

#endif//INCLUDED_NETWORK_CREATE_ACTOR_MESSAGE_SENDER_SYSTEM_H
