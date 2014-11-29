#include "platform/i_platform.h"
#include "core/pickup.h"
#include "core/i_inventory_component.h"
#include "core/i_health_component.h"
#include "core/i_collision_component.h"
#include "core/pickup_collision_component.h"

Pickup::Pickup( std::string const& Name )
    : Actor( AutoId(Name) )
{
    AddComponent( mComponentFactory(AutoId("pickup_collision_component")));
    Opt<PickupCollisionComponent> collisionC = Get<PickupCollisionComponent>();
    collisionC->SetRadius(0.04);
    collisionC->SetCollisionClass(CollisionClass::Pickup);
    collisionC->SetPickupContent( AutoId(Name) );

    AddComponent(ComponentFactory::Get()(AutoId("health_delete_component")));

    AddAction( AutoId( "fade_out" ) );

}
