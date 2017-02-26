#ifndef INCLUDED_NETWORK_LIFECYCLE_MESSAGE_HANDLER_SUB_SYSTEM_H
#define INCLUDED_NETWORK_LIFECYCLE_MESSAGE_HANDLER_SUB_SYSTEM_H
#include "message_handler_sub_system.h"
#include "core/map_start_event.h"

namespace network {

class LifecycleMessageHandlerSubSystem: public MessageHandlerSubSystem
{
    DEFINE_SUB_SYSTEM_BASE( LifecycleMessageHandlerSubSystem )
public:
    LifecycleMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute( Message const& message );
private:
    AutoReg mOnMapStart;
    void OnMapStart( core::MapStartEvent const& Evt );

};

} // namespace network

#endif//INCLUDED_NETWORK_LIFECYCLE_MESSAGE_HANDLER_SUB_SYSTEM_H
