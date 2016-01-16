#ifndef INCLUDED_NETWORK_SYNC_ITEM_H
#define INCLUDED_NETWORK_SYNC_ITEM_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "core/item.h"
#include "engine/item_properties_changed_event.h"

namespace network {

class SyncItemMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE(SyncItemMessage)
    int32_t mActorGUID;
    int32_t mItemID;
    std::string mData;
    SyncItemMessage( Item const& item );
    SyncItemMessage()
        : mActorGUID( 0 )
        , mItemID( 0 )
        , mData()
    {
    }
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void SyncItemMessage::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Message>(*this);
    ar & mActorGUID;
    ar & mItemID;
    ar & mData;
}

class SyncItemMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(SyncItemMessageHandlerSubSystem)
    virtual void Init();
    virtual void Execute(Message const& message);
};

class SyncItemMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnItemPropertiesChanged;
    void OnItemPropertiesChanged(engine::ItemPropertiesChangedEvent const& Evt);
public:
    DEFINE_SYSTEM_BASE(SyncItemMessageSenderSystem)
    SyncItemMessageSenderSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};
} // namespace network

#endif//INCLUDED_NETWORK_SYNC_ITEM_H
//command:  "../../../Reaping2_build/build/tools/classgenerator/classgenerator" -g "message" -c "sync_item" -p "pending" -m "int32_t-actorGUID int32_t-itemID std::string-data" -e "engine-itemPropertiesChanged"
