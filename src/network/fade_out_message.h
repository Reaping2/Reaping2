#ifndef INCLUDED_NETWORK_FADE_OUT_H
#define INCLUDED_NETWORK_FADE_OUT_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include <boost/serialization/export.hpp>

namespace network {

class FadeOutMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(FadeOutMessage)
    int32_t mActorGUID;
    double mSecsToEnd;
    FadeOutMessage()
        : mActorGUID(-1)
        , mSecsToEnd(0.0)
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void FadeOutMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
    ar & mActorGUID;
    ar & mSecsToEnd;
}

class FadeOutMessageHandlerSubSystem : public PendingMessageHandlerSubSystem<FadeOutMessage>
{
public:
    DEFINE_SUB_SYSTEM_BASE(FadeOutMessageHandlerSubSystem)
    FadeOutMessageHandlerSubSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
    virtual bool ProcessPending(Message const& message);
};

class FadeOutMessageSenderSystem : public MessageSenderSystem
{
public:
    DEFINE_SYSTEM_BASE(FadeOutMessageSenderSystem)
    FadeOutMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
    static std::auto_ptr<FadeOutMessage> GenerateFadeOutMessage(Actor &actor);
};
} // namespace network


BOOST_CLASS_EXPORT_KEY2(network::FadeOutMessage,"fade_out");
#endif//INCLUDED_NETWORK_FADE_OUT_H

//command:  "classgenerator.exe" -g "message" -c "fade_out" -m "int32_t-actorGUID double-secsToEnd" -t "i_fade_out_component" -p "pending"
