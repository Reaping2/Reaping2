#ifndef INCLUDED_NETWORK_REVIVE_MESSAGE_H
#define INCLUDED_NETWORK_REVIVE_MESSAGE_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "input/keyboard.h"
#include "platform/register.h"
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
        AutoReg mKeyId;
        bool mIsRivevePressed;
        void OnKeyEvent( const KeyEvent& Event );
    public:
        DEFINE_SYSTEM_BASE(ReviveMessageSenderSystem)
        ReviveMessageSenderSystem();
        virtual void Init();
        virtual void Update( double DeltaTime );
    };

} // namespace network
#endif//INCLUDED_NETWORK_REVIVE_MESSAGE_H
