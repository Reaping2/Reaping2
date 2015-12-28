#ifndef INCLUDED_NETWORK_CLIENT_LIST_MESSAGE_HANDLER_SUB_SYSTEM_H
#define INCLUDED_NETWORK_CLIENT_LIST_MESSAGE_HANDLER_SUB_SYSTEM_H

#include "message_handler_sub_system.h"

namespace network {

class ClientListMessageHandlerSubSystem : public MessageHandlerSubSystem
{
    DEFINE_SUB_SYSTEM_BASE(ClientListMessageHandlerSubSystem)
public:
    ClientListMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const & message);
};

}

#endif
