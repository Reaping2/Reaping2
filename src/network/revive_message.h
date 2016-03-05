#ifndef INCLUDED_NETWORK_REVIVE_MESSAGE_H
#define INCLUDED_NETWORK_REVIVE_MESSAGE_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "platform/register.h"
#include "platform/export.h"
namespace network {

    class ReviveMessage: public Message
    {
        friend class ::boost::serialization::access;
    public:
        DEFINE_MESSAGE_BASE(ReviveMessage)
        ReviveMessage()
        {
        }
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version);
    };

    template<class Archive>
    void ReviveMessage::serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Message>(*this);
    }

    class ReviveMessageHandlerSubSystem: public MessageHandlerSubSystem
    {
    public:
        DEFINE_SUB_SYSTEM_BASE(ReviveMessageHandlerSubSystem)
        ReviveMessageHandlerSubSystem();
        virtual void Init();
        virtual void Execute(Message const& message );
    };

    class ReviveMessageSenderSystem: public MessageSenderSystem
    {
    public:
        DEFINE_SYSTEM_BASE(ReviveMessageSenderSystem)
        ReviveMessageSenderSystem();
        virtual void Init();
        virtual void Update( double DeltaTime );
    };

} // namespace network

REAPING2_CLASS_EXPORT_KEY2(network__ReviveMessage, network::ReviveMessage,"revive");
#endif//INCLUDED_NETWORK_REVIVE_MESSAGE_H
