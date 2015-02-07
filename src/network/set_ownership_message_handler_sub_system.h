#ifndef INCLUDED_NETWORK_SET_OWNERSHIP_MESSAGE_HANDLER_SUB_SYSTEM_H
#define INCLUDED_NETWORK_SET_OWNERSHIP_MESSAGE_HANDLER_SUB_SYSTEM_H
#include "message_handler_sub_system.h"
#include "messsage_holder.h"
#include "core/scene.h"

namespace network {

    class SetOwnershipMessageHandlerSubSystem: public MessageHandlerSubSystem
    {
    public:
        DEFINE_SUB_SYSTEM_BASE(SetOwnershipMessageHandlerSubSystem)
        SetOwnershipMessageHandlerSubSystem();
        virtual void Init();
        virtual void Execute(Message const& message );
    };

} // namespace network

#endif//INCLUDED_NETWORK_SET_OWNERSHIP_MESSAGE_HANDLER_SUB_SYSTEM_H
