#include "platform/i_platform.h"
#include "engine/collisions/pickup_collision_sub_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "core/pickup_collision_component.h"
#include "core/i_inventory_component.h"
#include "core/i_health_component.h"
#include "core/buffs/i_buff_holder_component.h"
#include "core/buffs/buff_factory.h"
#include "core/item_type.h"

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
}

void PickupCollisionSubSystem::Collide(Actor& actor, Actor& other)
{
    Opt<PickupCollisionComponent> pickupCC = actor.Get<PickupCollisionComponent>();

    Opt<IInventoryComponent> inventoryC = other.Get<IInventoryComponent>();
    if (inventoryC.IsValid())
    {
        if (pickupCC->GetItemType()==ItemType::Weapon)
        {
            inventoryC->DropItemType( pickupCC->GetItemType() );
            inventoryC->AddItem( pickupCC->GetPickupContent() );
            inventoryC->SetSelectedWeapon( pickupCC->GetPickupContent() );
        }
        else if (pickupCC->GetItemType()==ItemType::Normal)
        {
            inventoryC->DropItemType( pickupCC->GetItemType() );
            inventoryC->AddItem( pickupCC->GetPickupContent() );
            inventoryC->SetSelectedNormalItem( pickupCC->GetPickupContent() );
        }
        else if (pickupCC->GetItemType()==ItemType::Buff)
        {
            Opt<IBuffHolderComponent> buffHolderC=other.Get<IBuffHolderComponent>();
            if (buffHolderC.IsValid())
            {
                buffHolderC->AddBuff(core::BuffFactory::Get()(pickupCC->GetPickupContent()));
            }
        }
        EventServer<PickupEvent>::Get().SendEvent(PickupEvent(Opt<Actor>(&other),pickupCC->GetItemType(),pickupCC->GetPickupContent()));
    }
    Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
    if (healthC.IsValid())
    {
        healthC->SetHP(0);
    }

}

} // namespace engine

