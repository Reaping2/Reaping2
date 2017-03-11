#include "network/pickup_message.h"
#include "core/i_inventory_component.h"
#include "core/actor_event.h"
#include "core/pickup_collision_component.h"
#include "core/item.h"
#include "core/buffs/i_buff_holder_component.h"
#include "platform/auto_id.h"
#include "core/buffs/buff_factory.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
namespace network {

PickupMessageSenderSystem::PickupMessageSenderSystem()
    : MessageSenderSystem()
{
}

void PickupMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    mOnPickup = EventServer<engine::PickupEvent>::Get().Subscribe( boost::bind( &PickupMessageSenderSystem::OnPickup, this, _1 ) );
}

void PickupMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
}

void PickupMessageSenderSystem::OnPickup( engine::PickupEvent const& Evt )
{
    std::auto_ptr<PickupMessage> pickupMsg( new PickupMessage );
    pickupMsg->mActorGUID = Evt.mActor->GetGUID();
    pickupMsg->mItemType = Evt.mItemType;
    pickupMsg->mItemId = Evt.mItemId;
    mMessageHolder.AddOutgoingMessage( pickupMsg );
}

PickupMessageHandlerSubSystem::PickupMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{

}

void PickupMessageHandlerSubSystem::Init()
{

}

void PickupMessageHandlerSubSystem::Execute( Message const& message )
{
    PickupMessage const& msg = static_cast<PickupMessage const&>( message );
    Opt<Actor> actor = mScene.GetActor( msg.mActorGUID );
    if ( !actor.IsValid() )
    {
        L1( "cannot find actor with GUID: (pickup) %d \n", msg.mActorGUID );
        return;
    }

    Opt<IInventoryComponent> inventoryC = actor->Get<IInventoryComponent>();
    if ( inventoryC.IsValid() )
    {
        L2( "pickup picked up with itemtype:%d,itemid:%d", msg.mItemType, msg.mItemId );

        //TODO item selection will be syncronized, not pickup event
        if ( msg.mItemType == ItemType::Weapon
            || msg.mItemType == ItemType::Normal )
        {
            inventoryC->AddItem( msg.mItemId );
            inventoryC->SetSelectedItem( msg.mItemType, msg.mItemId );
        }
        else if ( msg.mItemType == ItemType::Buff )
        {
            Opt<IBuffHolderComponent> buffHolderC = actor->Get<IBuffHolderComponent>();
            if ( buffHolderC.IsValid() )
            {
                buffHolderC->AddBuff( core::BuffFactory::Get()( msg.mItemId ) );
            }
        }

    }
}

} // namespace engine


REAPING2_CLASS_EXPORT_IMPLEMENT( network__PickupMessage, network::PickupMessage );
