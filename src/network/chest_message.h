#ifndef INCLUDED_NETWORK_CHEST_H
#define INCLUDED_NETWORK_CHEST_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "platform/export.h"
#include "core/chest_changed_event.h"

namespace network {

class ChestMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(ChestMessage)
    engine::ChestChangedEvent mEvent;
    ChestMessage()
        : mEvent()
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void ChestMessage::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Message>(*this);
    ar& mEvent;
}

class ChestMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(ChestMessageHandlerSubSystem)
    ChestMessageHandlerSubSystem();
    virtual void Init();
    virtual void Execute(Message const& message );
    virtual void Update(double DeltaTime);
};

class ChestMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnChestChanged;
    void OnChestChanged(engine::ChestChangedEvent const& Evt);
public:
    DEFINE_SYSTEM_BASE(ChestMessageSenderSystem)
    ChestMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace network

#endif//INCLUDED_NETWORK_CHEST_H

REAPING2_CLASS_EXPORT_KEY2( network__ChestMessage, network::ChestMessage, "chest" );

//command:  "classgenerator.exe" -g "message" -c "chest" -m "ChestChangedEvent-event" -e "engine-chestChanged"
