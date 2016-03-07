#ifndef INCLUDED_NETWORK_POSITION_MESSAGE_HANDLER_SUB_SYSTEM_H
#define INCLUDED_NETWORK_POSITION_MESSAGE_HANDLER_SUB_SYSTEM_H
#include "message_handler_sub_system.h"

namespace network {

class PositionMessageHandlerSubSystem: public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( PositionMessageHandlerSubSystem )
    PositionMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute( Message const& message );
};

} // namespace network

#endif//INCLUDED_NETWORK_POSITION_MESSAGE_HANDLER_SUB_SYSTEM_H
