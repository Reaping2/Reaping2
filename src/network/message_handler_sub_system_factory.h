#ifndef INCLUDED_NETWORK_MESSAGE_HANDLER_SUB_SYSTEM_FACTORY_H
#define INCLUDED_NETWORK_MESSAGE_HANDLER_SUB_SYSTEM_FACTORY_H

#include "platform/factory.h"
#include "platform/singleton.h"

#include <memory>
#include "message_handler_sub_system.h"
namespace network {
class MessageHandlerSubSystemFactory : public platform::Factory<MessageHandlerSubSystem>, public platform::Singleton<MessageHandlerSubSystemFactory>
{
    friend class platform::Singleton<MessageHandlerSubSystemFactory>;
    MessageHandlerSubSystemFactory();
protected:
    template<typename Elem_T>
    static std::auto_ptr<MessageHandlerSubSystem> CreateSubSystem( int32_t Id );
};
template<typename Elem_T>
std::auto_ptr<MessageHandlerSubSystem> MessageHandlerSubSystemFactory::CreateSubSystem( int32_t Id )
{
    return std::auto_ptr<MessageHandlerSubSystem>( new Elem_T() );
}
} // namespace engine
#endif//INCLUDED_NETWORK_MESSAGE_HANDLER_SUB_SYSTEM_FACTORY_H