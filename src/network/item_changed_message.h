#ifndef INCLUDED_NETWORK_ITEM_CHANGED_H
#define INCLUDED_NETWORK_ITEM_CHANGED_H

#include "network/message.h"
#include "network/message_handler_sub_system.h"
#include "network/message_sender_system.h"
#include "engine/item_changed_event.h"
#include "engine/soldier_created_event.h"
#include "platform/export.h"

namespace network {

class ItemChangedMessage : public Message
{
    friend class ::boost::serialization::access;
public:
    DEFINE_MESSAGE_BASE( ItemChangedMessage )
    int32_t mActorGUID;
    int32_t mNormalId;
    int32_t mWeaponId;
    ItemChangedMessage()
        : mActorGUID( -1 )
        , mNormalId( 0 )
        , mWeaponId( 0 )
    {
    }
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ItemChangedMessage::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Message>( *this );
    ar& mActorGUID;
    ar& mNormalId;
    ar& mWeaponId;
}

class ItemChangedMessageHandlerSubSystem : public PendingMessageHandlerSubSystem<ItemChangedMessage>
{
public:
    DEFINE_SUB_SYSTEM_BASE( ItemChangedMessageHandlerSubSystem )
    ItemChangedMessageHandlerSubSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
    virtual bool ProcessPending( Message const& message );
};

class ItemChangedMessageSenderSystem : public MessageSenderSystem
{
    AutoReg mOnItemChanged;
    void OnItemChanged( engine::ItemChangedEvent const& Evt );
    AutoReg mOnSoldierCreated;
    void OnSoldierCreated( engine::SoldierCreatedEvent const& Evt );
public:
    DEFINE_SYSTEM_BASE( ItemChangedMessageSenderSystem )
    ItemChangedMessageSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
};
} // namespace network


REAPING2_CLASS_EXPORT_KEY2( network__ItemChangedMessage, network::ItemChangedMessage, "item_changed" );
#endif//INCLUDED_NETWORK_ITEM_CHANGED_H

//command:  "classgenerator.exe" -g "message" -c "item_changed" -e "engine-itemChanged" -m "int32_t-actorGUID int32_t-normalId int32_t-weaponId" -p "pending"
