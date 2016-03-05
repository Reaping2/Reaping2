#ifndef INCLUDED_NETWORK_SECS_TO_REVIVE_H
#define INCLUDED_NETWORK_SECS_TO_REVIVE_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "engine/secs_to_revive_event.h"
#include "platform/export.h"

namespace network {

class SecsToReviveMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(SecsToReviveMessage)
    int32_t mClientId;
    double mSecsToRevive;
    SecsToReviveMessage()
        : mClientId(-1)
        , mSecsToRevive(0.0)
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void SecsToReviveMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
    ar & mClientId;
    ar & mSecsToRevive;
}

class SecsToReviveMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(SecsToReviveMessageHandlerSubSystem)
    SecsToReviveMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
    virtual void Update(double DeltaTime);
};

class SecsToReviveMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnSecsToRevive;
    void OnSecsToRevive(engine::SecsToReviveEvent const& Evt);
public:
    DEFINE_SYSTEM_BASE(SecsToReviveMessageSenderSystem)
    SecsToReviveMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace network


REAPING2_CLASS_EXPORT_KEY2(network__SecsToReviveMessage, network::SecsToReviveMessage,"secs_to_revive");
#endif//INCLUDED_NETWORK_SECS_TO_REVIVE_H

//command:  "classgenerator" -g "message" -c "secs_to_revive" -e "engine-secsToRevive" -m "int32_t-clientId double-secsToRevive"
