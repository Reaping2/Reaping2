#ifndef INCLUDED_NETWORK_BORDER_H
#define INCLUDED_NETWORK_BORDER_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "core/i_border_component.h"

namespace network {

class BorderMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(BorderMessage)
    int32_t mActorGUID;
    IBorderComponent::Borders_t mBorders;
    BorderMessage()
        : mActorGUID(-1)
        , mBorders()
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

#endif//INCLUDED_NETWORK_BORDER_H

//command:  "classgenerator.exe" -g "message" -c "border" -p "pending" -t "border" -m "int32_t-actorGUID IBorderComponent::Borders_t-borders"
