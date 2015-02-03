#ifndef INCLUDED_NETWORK_CLIENT_ID_MESSAGE_HANDLER_SUB_SYSTEM_H
#define INCLUDED_NETWORK_CLIENT_ID_MESSAGE_HANDLER_SUB_SYSTEM_H
#include "message_handler_sub_system.h"

namespace network {

class ClientIdMessageHandlerSubSystem: public MessageHandlerSubSystem
{
    DEFINE_SUB_SYSTEM_BASE(ClientIdMessageHandlerSubSystem)
public:
    ClientIdMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
};

} // namespace network

#endif//INCLUDED_NETWORK_CLIENT_ID_MESSAGE_HANDLER_SUB_SYSTEM_H
