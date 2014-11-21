#include "i_core.h"
#include "core/i_inventory_component.h"
#include "core/i_health_component.h"

Pickup::Pickup( std::string const& Name )
    : Actor( Name )
{
    mFields[COLLISION_CLASS].i = CollisionClass::Pickup;
    mFields[RADIUS].d = 0.04;
    AddAction( AutoId( "fade_out" ) );
    AddComponent(ComponentFactory::Get()(AutoId("health_delete_component")));
    Get<IHealthComponent>()->SetActor(this);

}

void Pickup::Collide( Actor& Other )
{
    //TODO: action! this should not make this fun here
    Opt<IInventoryComponent> inventoryC = Other.Get<IInventoryComponent>();
    if (inventoryC.IsValid())
    {
        inventoryC->DropItemType( Item::Weapon );
        inventoryC->AddItem( GetId() );
    }
    Get<IHealthComponent>()->SetHP(0);
}