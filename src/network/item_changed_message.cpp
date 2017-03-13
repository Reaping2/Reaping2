#include "platform/i_platform.h"
#include "network/item_changed_message.h"
#include "core/i_inventory_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
#include "engine/item_changed_event.h"

namespace network {

ItemChangedMessageSenderSystem::ItemChangedMessageSenderSystem()
    : MessageSenderSystem()
{
}


void ItemChangedMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnItemChanged = EventServer<engine::ItemChangedEvent>::Get().Subscribe( boost::bind( &ItemChangedMessageSenderSystem::OnItemChanged, this, _1 ) );
    mOnSoldierCreated = EventServer<engine::SoldierCreatedEvent>::Get().Subscribe( boost::bind( &ItemChangedMessageSenderSystem::OnSoldierCreated, this, _1 ) );
}


void ItemChangedMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
}

void ItemChangedMessageSenderSystem::OnItemChanged( engine::ItemChangedEvent const& Evt )
{
    std::auto_ptr<ItemChangedMessage> itemChangedMsg( new ItemChangedMessage );
    itemChangedMsg->mActorGUID = Evt.mActorGUID;
    itemChangedMsg->mType = Evt.mType;
    itemChangedMsg->mItemId = Evt.mItemId;
    itemChangedMsg->mPrevItemId = Evt.mPrevItemId;
    itemChangedMsg->mDropPrevItem = Evt.mDropPrevItem;
    mMessageHolder.AddOutgoingMessage( itemChangedMsg );
}

void ItemChangedMessageSenderSystem::OnSoldierCreated( engine::SoldierCreatedEvent const& Evt )
{
    if (Evt.mState != engine::SoldierCreatedEvent::Finished)
    {
        return;
    }
    Opt<IInventoryComponent> inventoryC = Evt.mActor->Get<IInventoryComponent>();
    if ( !inventoryC.IsValid() )
    {
        return;
    }
    Opt<NormalItem> item = inventoryC->GetSelectedItem( ItemType::Normal );
    if (item.IsValid())
    {
        std::auto_ptr<ItemChangedMessage> itemChangedMsg( new ItemChangedMessage );
        itemChangedMsg->mActorGUID = Evt.mActor->GetGUID();
        itemChangedMsg->mType = item->GetType();
        itemChangedMsg->mItemId = item->GetId();
        mMessageHolder.AddOutgoingMessage( itemChangedMsg );
    }
    Opt<Weapon> weapon = inventoryC->GetSelectedItem( ItemType::Weapon );
    if (weapon.IsValid())
    {
        std::auto_ptr<ItemChangedMessage> itemChangedMsg( new ItemChangedMessage );
        itemChangedMsg->mActorGUID = Evt.mActor->GetGUID();
        itemChangedMsg->mType = weapon->GetType();
        itemChangedMsg->mItemId = weapon->GetId();
        mMessageHolder.AddOutgoingMessage( itemChangedMsg );
    }
}

ItemChangedMessageHandlerSubSystem::ItemChangedMessageHandlerSubSystem()
    : PendingMessageHandlerSubSystem()
{
}


void ItemChangedMessageHandlerSubSystem::Init()
{
}

void ItemChangedMessageHandlerSubSystem::Update( double DeltaTime )
{
    PendingMessageHandlerSubSystem::Update( DeltaTime );
}

bool ItemChangedMessageHandlerSubSystem::ProcessPending( Message const& message )
{
    ItemChangedMessage const& msg = static_cast<ItemChangedMessage const&>( message );
    Opt<Actor> actor = mScene.GetActor( msg.mActorGUID ); //guaranteed
    L2( "executing %s: actorGUID %d \n", __FUNCTION__, msg.mActorGUID );
    Opt<IInventoryComponent> inventoryC = actor->Get<IInventoryComponent>();
    if ( !inventoryC.IsValid() )
    {
        L1( "actor inventory not valid! returning." );
        return true;
    }
    if ( msg.mDropPrevItem )
    {
        inventoryC->DropItem( msg.mPrevItemId );
    }

    if (msg.mType == ItemType::Normal
        || msg.mType == ItemType::Weapon)
    {
        inventoryC->SetSelectedItem( msg.mType, msg.mItemId, true );
    }
    EventServer<engine::ItemChangedEvent>::Get().SendEvent( { msg.mActorGUID, msg.mType, msg.mItemId, msg.mPrevItemId, msg.mDropPrevItem } );
    return true;
}

} // namespace network


REAPING2_CLASS_EXPORT_IMPLEMENT( network__ItemChangedMessage, network::ItemChangedMessage );
