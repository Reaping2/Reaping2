#include "platform/i_platform.h"
#include "engine/collisions/pickup_collision_sub_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "core/pickup_collision_component.h"
#include "core/i_inventory_component.h"
#include "core/i_health_component.h"

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
        inventoryC->DropItemType( pickupCC->GetItemType() );
        inventoryC->AddItem( pickupCC->GetPickupContent() );
        if (pickupCC->GetItemType()==Item::Weapon)
        {
            inventoryC->SetSelectedWeapon( pickupCC->GetPickupContent() );
        }
        else if (pickupCC->GetItemType()==Item::Normal)
        {
            inventoryC->SetSelectedNormalItem( pickupCC->GetPickupContent() );
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

