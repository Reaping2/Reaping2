#ifndef INCLUDED_NETWORK_SOLDIER_PROPERTIES_H
#define INCLUDED_NETWORK_SOLDIER_PROPERTIES_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "core/actor_event.h"
#include "engine/soldier_properties_ready_event.h"
#include "engine/soldier_created_event.h"

using core::SoldierProperties;
namespace network {

class SoldierPropertiesMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(SoldierPropertiesMessage)
    SoldierProperties mSoldierProperties;
    SoldierPropertiesMessage()
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void SoldierPropertiesMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
    ar & mSoldierProperties;
}

class SoldierPropertiesMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(SoldierPropertiesMessageHandlerSubSystem)
    SoldierPropertiesMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
};

class SoldierPropertiesMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnSoldierCreatedEvent;
    void OnSoldierCreatedEvent(engine::SoldierCreatedEvent const& Evt);
    AutoReg mOnSoldierPropertiesReady;
    void OnSoldierPropertiesReady( engine::SoldierPropertiesReadyEvent const& Evt );
public:
    DEFINE_SYSTEM_BASE(SoldierPropertiesMessageSenderSystem)
    SoldierPropertiesMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);

};
} // namespace network

#endif//INCLUDED_NETWORK_SOLDIER_PROPERTIES_H

