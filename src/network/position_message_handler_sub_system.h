#ifndef INCLUDED_NETWORK_POSITION_MESSAGE_HANDLER_SUB_SYSTEM_H
#define INCLUDED_NETWORK_POSITION_MESSAGE_HANDLER_SUB_SYSTEM_H
#include "message_handler_sub_system.h"
#include "messsage_holder.h"
#include "core/scene.h"

namespace network {

    class PositionMessageHandlerSubSystem: public MessageHandlerSubSystem
    {
        MessageHolder& mMessageHolder;
        Scene& mScene;
    public:
        DEFINE_SUB_SYSTEM_BASE(PositionMessageHandlerSubSystem)
        PositionMessageHandlerSubSystem();
        virtual void Init();
        virtual void Execute(Message const& message );
    };

} // namespace network

#endif//INCLUDED_NETWORK_POSITION_MESSAGE_HANDLER_SUB_SYSTEM_H
