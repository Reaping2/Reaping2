#ifndef INCLUDED_NETWORK_MESSAGE_FACTORY_H
#define INCLUDED_NETWORK_MESSAGE_FACTORY_H

#include "platform/factory.h"
#include "platform/singleton.h"

#include <memory>
#include "message.h"
namespace network {
class MessageFactory : public platform::Factory<Message>, public platform::Singleton<MessageFactory>
{
    friend class platform::Singleton<MessageFactory>;
    MessageFactory();
protected:
    template<typename Elem_T>
    static std::auto_ptr<Message> CreateMessage( int32_t Id );
};
template<typename Elem_T>
std::auto_ptr<Message> MessageFactory::CreateMessage( int32_t Id )
{
    return std::auto_ptr<Message>( new Elem_T() );
}
} // namespace engine
#endif//INCLUDED_NETWORK_MESSAGE_FACTORY_H