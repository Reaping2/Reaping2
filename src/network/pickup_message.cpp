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
        mOnPickup= EventServer<engine::PickupEvent>::Get().Subscribe( boost::bind( &PickupMessageSenderSystem::OnPickup, this, _1 ) );
        mOnActorEvent = EventServer<ActorEvent>::Get().Subscribe( boost::bind( &PickupMessageSenderSystem::OnActorEvent, this, _1 ) );
    }

    void PickupMessageSenderSystem::Update(double DeltaTime)
    {
        MessageSenderSystem::Update(DeltaTime);
    }

    void PickupMessageSenderSystem::OnActorEvent(ActorEvent const& Evt)
    {
        if(Evt.mState==ActorEvent::Added)
        {
            Opt<PickupCollisionComponent> pickupCC = Evt.mActor->Get<PickupCollisionComponent>();
            if(!pickupCC.IsValid())
            {
                return;
            }
            std::auto_ptr<SetPickupContentMessage> setPickupMsg(new SetPickupContentMessage);
            setPickupMsg->mActorGUID=Evt.mActor->GetGUID();
            setPickupMsg->mContentId=pickupCC->GetPickupContent();
            setPickupMsg->mItemType=pickupCC->GetItemType();
            mMessageHolder.AddOutgoingMessage(setPickupMsg);
        }
    }

    void PickupMessageSenderSystem::OnPickup(engine::PickupEvent const& Evt)
    {
        std::auto_ptr<PickupMessage> pickupMsg(new PickupMessage);
        pickupMsg->mActorGUID=Evt.mActor->GetGUID();
        pickupMsg->mItemType=Evt.mItemType;
        pickupMsg->mItemId=Evt.mItemId;
        mMessageHolder.AddOutgoingMessage(pickupMsg);
    }

    PickupMessageHandlerSubSystem::PickupMessageHandlerSubSystem()
        : MessageHandlerSubSystem()
    {

    }

    void PickupMessageHandlerSubSystem::Init()
    {

    }

    void PickupMessageHandlerSubSystem::Execute(Message const& message)
    {
        PickupMessage const& msg=static_cast<PickupMessage const&>(message);
        Opt<Actor> actor=mScene.GetActor(msg.mActorGUID);
        if (!actor.IsValid())
        {
            L1("cannot find actor with GUID: (pickup) %d \n",msg.mActorGUID );
            return;
        }

        Opt<IInventoryComponent> inventoryC = actor->Get<IInventoryComponent>();
        if (inventoryC.IsValid())
        {
            L2("pickup picked up with itemtype:%d,itemid:%d",msg.mItemType,msg.mItemId);

            //TODO item selection will be syncronized, not pickup event
            if (msg.mItemType==ItemType::Weapon)
            {
                inventoryC->DropItemType( msg.mItemType );
                inventoryC->AddItem( msg.mItemId );
                inventoryC->SetSelectedWeapon( msg.mItemId );
            }
            else if (msg.mItemType==ItemType::Normal)
            {
                inventoryC->DropItemType( msg.mItemType );
                inventoryC->AddItem( msg.mItemId );
                inventoryC->SetSelectedNormalItem( msg.mItemId );
            }
            else if (msg.mItemType==ItemType::Buff)
            {
                Opt<IBuffHolderComponent> buffHolderC=actor->Get<IBuffHolderComponent>();
                if (buffHolderC.IsValid())
                {
                    buffHolderC->AddBuff(core::BuffFactory::Get()(msg.mItemId));
                }
            }

        }
    }

    SetPickupContentMessageHandlerSubSystem::SetPickupContentMessageHandlerSubSystem()
        : MessageHandlerSubSystem()
    {

    }

    void SetPickupContentMessageHandlerSubSystem::Init()
    {

    }

    void SetPickupContentMessageHandlerSubSystem::Execute(Message const& message)
    {
        SetPickupContentMessage const& msg=static_cast<SetPickupContentMessage const&>(message);
        L1("executing SetPickupContent: %d \n",msg.mSenderId );
        Opt<Actor> actor=mScene.GetActor(msg.mActorGUID);
        if (!actor.IsValid())
        {
            L1("cannot find actor with GUID: (set_pickup_content) %d \n",msg.mActorGUID );
            return;
        }

        Opt<PickupCollisionComponent> pickupCC = actor->Get<PickupCollisionComponent>();
        if(pickupCC.IsValid())
        {
            pickupCC->SetPickupContent(msg.mContentId);
            pickupCC->SetItemType(msg.mItemType);
        }
    }

} // namespace engine


BOOST_CLASS_EXPORT_IMPLEMENT(network::PickupMessage);

BOOST_CLASS_EXPORT_IMPLEMENT(network::SetPickupContentMessage);
