#ifndef INCLUDED_NETWORK_MOVE_MESSAGE_HANDLER_SUB_SYSTEM_H
#define INCLUDED_NETWORK_MOVE_MESSAGE_HANDLER_SUB_SYSTEM_H
#include "message_handler_sub_system.h"

namespace network {

    class MoveMessageHandlerSubSystem: public MessageHandlerSubSystem
    {
    public:
        DEFINE_SUB_SYSTEM_BASE(MoveMessageHandlerSubSystem)
        MoveMessageHandlerSubSystem();
        virtual void Init();
        virtual void Execute(Message const& message );
    };

} // namespace network

#endif//INCLUDED_NETWORK_MOVE_MESSAGE_HANDLER_SUB_SYSTEM_H
