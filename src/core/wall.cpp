#include "platform/i_platform.h"
#include "core/wall.h"
#include "core/i_collision_component.h"

Wall::Wall( std::string const& Name )
    : Actor( AutoId(Name) )
{
    AddComponent( mComponentFactory(AutoId("wall_collision_component")));
    Opt<ICollisionComponent> collisionC = Get<ICollisionComponent>();
    collisionC->SetRadius(0.1);
    collisionC->SetCollisionClass(CollisionClass::Wall);
}
