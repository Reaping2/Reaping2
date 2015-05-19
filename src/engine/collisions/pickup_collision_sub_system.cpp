#include "platform/i_platform.h"
#include "engine/collisions/pickup_collision_sub_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "core/pickup_collision_component.h"
#include "core/i_inventory_component.h"
#include "core/i_health_component.h"
#include "core/buffs/i_buff_holder_component.h"
#include "core/buffs/buff_factory.h"

namespace engine {

PickupCollisionSubSystem::PickupCollisionSubSystem()
    : CollisionSubSystem()
{

}

void PickupCollisionSubSystem::Init()
{
}

void PickupCollisionSubSystem::Update(Actor& actor, double DeltaTime)
{
    if (!mOther)
    {
        return;
    }

    Opt<PickupCollisionComponent> pickupCC = actor.Get<PickupCollisionComponent>();

    Opt<IInventoryComponent> inventoryC = mOther->Get<IInventoryComponent>();
    if (inventoryC.IsValid())
    {
        if (pickupCC->GetItemType()==Item::Weapon)
        {
            inventoryC->DropItemType( pickupCC->GetItemType() );
            inventoryC->AddItem( pickupCC->GetPickupContent() );
            inventoryC->SetSelectedWeapon( pickupCC->GetPickupContent() );
        }
        else if (pickupCC->GetItemType()==Item::Normal)
        {
            inventoryC->DropItemType( pickupCC->GetItemType() );
            inventoryC->AddItem( pickupCC->GetPickupContent() );
            inventoryC->SetSelectedNormalItem( pickupCC->GetPickupContent() );
        }
        else if (pickupCC->GetItemType()==Item::Buff)
        {
            Opt<IBuffHolderComponent> buffHolderC=mOther->Get<IBuffHolderComponent>();
            if (buffHolderC.IsValid())
            {
                buffHolderC->AddBuff(core::BuffFactory::Get()(pickupCC->GetPickupContent()));
            }
        }
        EventServer<PickupEvent>::Get().SendEvent(PickupEvent(Opt<Actor>(mOther),pickupCC->GetItemType(),pickupCC->GetPickupContent()));
    }
    Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
    if (healthC.IsValid())
    {
        healthC->SetHP(0);
    }

}
} // namespace engine

