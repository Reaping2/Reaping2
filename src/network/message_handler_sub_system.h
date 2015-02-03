#ifndef INCLUDED_NETWORK_MESSAGE_HANDLER_SUB_SYSTEM_H
#define INCLUDED_NETWORK_MESSAGE_HANDLER_SUB_SYSTEM_H

#include "message.h"
#include "engine/sub_system.h"

namespace network {

    class MessageHandlerSubSystem
    {
    public:
        virtual int32_t GetType() const=0;
        virtual void Init()=0;
        virtual void Execute( Message const&  message )=0;
        virtual ~MessageHandlerSubSystem();
    };

    class DefaultMessageHandlerSubSystem : public MessageHandlerSubSystem
    {
    public:
        DEFINE_SUB_SYSTEM_BASE(DefaultMessageHandlerSubSystem)
        virtual void Init();
        virtual void Execute( Message const&  message );
    };
} // namespace engine

#endif//INCLUDED_NETWORK_MESSAGE_HANDLER_SUB_SYSTEM_H