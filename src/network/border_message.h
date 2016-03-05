#ifndef INCLUDED_NETWORK_BORDER_H
#define INCLUDED_NETWORK_BORDER_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "core/i_border_component.h"
#include "platform/export.h"

namespace network {

class BorderMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(BorderMessage)
    int32_t mActorGUID;
    IBorderComponent::Borders_t mBorders;
    IBorderComponent::Borders_t mOuterBorders;
    BorderMessage()
        : mActorGUID(-1)
        , mBorders()
        , mOuterBorders()
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void BorderMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
    ar & mActorGUID;
    ar & mBorders;
    ar & mOuterBorders;
}

class BorderMessageHandlerSubSystem : public PendingMessageHandlerSubSystem<BorderMessage>
{
public:
    DEFINE_SUB_SYSTEM_BASE(BorderMessageHandlerSubSystem)
    BorderMessageHandlerSubSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
    virtual bool ProcessPending(Message const& message);
};

class BorderMessageSenderSystem : public MessageSenderSystem
{
public:
    DEFINE_SYSTEM_BASE(BorderMessageSenderSystem)
    BorderMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
    static std::auto_ptr<BorderMessage> GenerateBorderMessage(Actor &actor);
};
} // namespace network


REAPING2_CLASS_EXPORT_KEY2(network__BorderMessage, network::BorderMessage,"border");
#endif//INCLUDED_NETWORK_BORDER_H

//command:  "classgenerator.exe" -g "message" -c "border" -p "pending" -t "border" -m "int32_t-actorGUID IBorderComponent::Borders_t-borders"
