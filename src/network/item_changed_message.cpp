#include "platform/i_platform.h"
#include "network/item_changed_message.h"
#include "core/i_inventory_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

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
    itemChangedMsg->mNormalId = Evt.mNormalId;
    itemChangedMsg->mWeaponId = Evt.mWeaponId;
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
    std::auto_ptr<ItemChangedMessage> itemChangedMsg( new ItemChangedMessage );
    itemChangedMsg->mActorGUID = Evt.mActor->GetGUID();
    Opt<NormalItem> item = inventoryC->GetSelectedNormalItem();
    itemChangedMsg->mNormalId = item.IsValid() ? item->GetId() : 0;
    Opt<Weapon> weapon = inventoryC->GetSelectedWeapon();
    itemChangedMsg->mWeaponId = weapon.IsValid() ? weapon->GetId() : 0;
    mMessageHolder.AddOutgoingMessage( itemChangedMsg );
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
    if ( msg.mNormalId == 0 )
    {
        inventoryC->DropItemType( ItemType::Normal );
    }
    inventoryC->SetSelectedNormalItem( msg.mNormalId );
    if ( msg.mWeaponId == 0 )
    {
        inventoryC->DropItemType( ItemType::Weapon );
    }
    inventoryC->SetSelectedWeapon( msg.mWeaponId );
    return true;
}

} // namespace network


REAPING2_CLASS_EXPORT_IMPLEMENT( network__ItemChangedMessage, network::ItemChangedMessage );
