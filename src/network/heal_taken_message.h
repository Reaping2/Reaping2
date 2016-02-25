#ifndef INCLUDED_NETWORK_HEAL_TAKEN_H
#define INCLUDED_NETWORK_HEAL_TAKEN_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "core/heal_taken_event.h"
#include <boost/serialization/export.hpp>

namespace network {

class HealTakenMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(HealTakenMessage)
    int32_t mX;
    int32_t mY;
    int32_t mHeal;
    int32_t mActorGUID;
    HealTakenMessage()
        : mX(0)
        , mY(0)
        , mHeal(0)
        , mActorGUID(0)
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void HealTakenMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
    ar & mX;
    ar & mY;
    ar & mHeal;
    ar & mActorGUID;
}

class HealTakenMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(HealTakenMessageHandlerSubSystem)
    HealTakenMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
};

class HealTakenMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnHealTaken;
    void OnHealTaken(core::HealTakenEvent const& Evt);
public:
    DEFINE_SYSTEM_BASE(HealTakenMessageSenderSystem)
    HealTakenMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace network


BOOST_CLASS_EXPORT_KEY2(network::HealTakenMessage,"heal_taken");
#endif//INCLUDED_NETWORK_HEAL_TAKEN_H
