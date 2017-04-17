#include "core/collision_traits.h"
#include "core/i_collision_component.h"

namespace core {

namespace {
const uint32_t Collisions[] =
{
    0,                                                                                                              // no collision
    1 << CollisionClass::Creep | 1 << CollisionClass::Mine | 1 << CollisionClass::Player | 1 << CollisionClass::Wall, // projectile
    1 << CollisionClass::Creep | 1 << CollisionClass::Player, // AoeDamage
    1 << CollisionClass::Projectile | 1 << CollisionClass::AoeDamage | 1 << CollisionClass::Mine | 1 << CollisionClass::Player | 1 << CollisionClass::Wall | 1 << CollisionClass::Water | 1 << CollisionClass::Pickup, // creep
    1 << CollisionClass::Projectile | 1 << CollisionClass::Creep | 1 << CollisionClass::Wall,                       // mine
    1 << CollisionClass::Projectile | 1 << CollisionClass::AoeDamage | 1 << CollisionClass::Creep | 1 << CollisionClass::Player | 1 << CollisionClass::Wall | 1 << CollisionClass::Pickup | 1 << CollisionClass::Water | 1 << CollisionClass::Flag, // player
    1 << CollisionClass::Projectile | 1 << CollisionClass::Creep | 1 << CollisionClass::Mine | 1 << CollisionClass::Player, // wall
    1 << CollisionClass::Creep | 1 << CollisionClass::Player, // water
    1 << CollisionClass::Creep | 1 << CollisionClass::Player, // pickup
    1 << CollisionClass::Player | 1 << CollisionClass::Flag, // flag
    0, // activatable
};



}

bool CollisionTraits::AreCorrelated( int32_t CC1, int32_t CC2 )
{
    return sizeof( Collisions ) / sizeof( Collisions[0] ) > CC1
        && 0 <= CC1
        && Collisions[ CC1 ] & ( 1 << CC2 );
}

int32_t CollisionTraits::GetTrait( Actor const& actor )
{
    Opt<ICollisionComponent> collC = actor.Get<ICollisionComponent>();
    if( !collC.IsValid() )
    {
        return CollisionClass::No_Collision;
    }
    return collC->GetCollisionClass();
}

double CollisionTraits::GetRadius( Actor const& actor )
{
    Opt<ICollisionComponent> collC = actor.Get<ICollisionComponent>();
    if( !collC.IsValid() )
    {
        return 0.0;
    }
    return collC->GetRadius();
}

}

