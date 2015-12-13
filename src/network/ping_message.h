#ifndef INCLUDED_NETWORK_PING_MESSAGE_H
#define INCLUDED_NETWORK_PING_MESSAGE_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
namespace network {

    class PingMessage: public Message
    {
        friend class ::boost::serialization::access;
    public:
        DEFINE_MESSAGE_BASE(PingMessage)
        double mCurrentTime;
        int32_t mClientId; //TODO: there will be a per client sending option. later
        PingMessage()
            : mCurrentTime(0.0)
            , mClientId(-1)
        {
        }
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version);
    };

    template<class Archive>
    void PingMessage::serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Message>(*this);
        ar & mCurrentTime;
        ar & mClientId;
    }

    class PingMessageHandlerSubSystem: public MessageHandlerSubSystem
    {
        int32_t mPing;
    public:
        ModelValue mPingModel;
        int32_t GetPing();
        DEFINE_SUB_SYSTEM_BASE(PingMessageHandlerSubSystem)
        PingMessageHandlerSubSystem();
        virtual void Init();
        virtual void Execute(Message const& message );
    };

    class PingMessageSenderSystem: public MessageSenderSystem
    {
    public:
        DEFINE_SYSTEM_BASE(PingMessageSenderSystem)
        PingMessageSenderSystem();
        virtual void Init();
        virtual void Update( double DeltaTime );
    };

} // namespace network
#endif//INCLUDED_NETWORK_PING_MESSAGE_H
