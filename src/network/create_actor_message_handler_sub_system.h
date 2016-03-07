#ifndef INCLUDED_NETWORK_CREATE_ACTOR_MESSAGE_HANDLER_SUB_SYSTEM_H
#define INCLUDED_NETWORK_CREATE_ACTOR_MESSAGE_HANDLER_SUB_SYSTEM_H
#include "message_handler_sub_system.h"
#include "messsage_holder.h"
#include "core/actor_factory.h"
#include "core/scene.h"

namespace network {

class CreateActorMessageHandlerSubSystem: public MessageHandlerSubSystem
{
    ActorFactory& mActorFactory;
public:
    DEFINE_SUB_SYSTEM_BASE( CreateActorMessageHandlerSubSystem )
    CreateActorMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute( Message const& message );
};

} // namespace network

#endif//INCLUDED_NETWORK_CREATE_ACTOR_MESSAGE_HANDLER_SUB_SYSTEM_H
