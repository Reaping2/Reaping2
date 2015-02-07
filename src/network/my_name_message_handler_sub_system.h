#ifndef INCLUDED_NETWORK_MY_NAME_MESSAGE_HANDLER_SUB_SYSTEM_H
#define INCLUDED_NETWORK_MY_NAME_MESSAGE_HANDLER_SUB_SYSTEM_H
#include "message_handler_sub_system.h"
#include "messsage_holder.h"

namespace network {

    class MyNameMessageHandlerSubSystem: public MessageHandlerSubSystem
    {
    public:
        DEFINE_SUB_SYSTEM_BASE(MyNameMessageHandlerSubSystem)
        MyNameMessageHandlerSubSystem();
        virtual void Init();
        virtual void Execute(Message const& message );
    };

} // namespace network

#endif//INCLUDED_NETWORK_MY_NAME_MESSAGE_HANDLER_SUB_SYSTEM_H
