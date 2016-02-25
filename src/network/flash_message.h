#ifndef INCLUDED_NETWORK_FLASH_H
#define INCLUDED_NETWORK_FLASH_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "engine/items/flash_event.h"
#include <boost/serialization/export.hpp>

namespace network {

class FlashMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(FlashMessage)
    int32_t mActorGUID;
    int32_t mOriginalX;
    int32_t mOriginalY;
    int32_t mX;
    int32_t mY;
    FlashMessage()
        : mActorGUID(0)
        , mOriginalX(0)
        , mOriginalY(0)
        , mX(0)
        , mY(0)
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void FlashMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
    ar & mActorGUID;
    ar & mOriginalX;
    ar & mOriginalY;
    ar & mX;
    ar & mY;
}

class FlashMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(FlashMessageHandlerSubSystem)
    FlashMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
};

class FlashMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnFlashEvent;
    void OnFlashEvent(engine::FlashEvent const& Evt);
public:
    DEFINE_SYSTEM_BASE(FlashMessageSenderSystem)
    FlashMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace network


BOOST_CLASS_EXPORT_KEY2(network::FlashMessage,"flash");
#endif//INCLUDED_NETWORK_FLASH_H
