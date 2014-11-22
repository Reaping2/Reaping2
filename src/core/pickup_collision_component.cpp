#include "core/i_inventory_component.h"
#include "core/i_health_component.h"
#include "core/pickup_collision_component.h"
#include "i_core.h" //TODO: actor sucks to include atm

PickupCollisionComponent::PickupCollisionComponent()
    : CollisionComponent()
    , mPickupContent( 0 )
{

}

void PickupCollisionComponent::Collide(Actor& Other)
{
    //TODO: action! this should not make this fun here
    Opt<IInventoryComponent> inventoryC = Other.Get<IInventoryComponent>();
    if (inventoryC.IsValid())
    {
        inventoryC->DropItemType( Item::Weapon );
        inventoryC->AddItem( mPickupContent );
    }
    mActor->Get<IHealthComponent>()->SetHP(0);
}

void PickupCollisionComponent::SetPickupContent(int32_t PickupContent)
{
    mPickupContent=PickupContent;
}
