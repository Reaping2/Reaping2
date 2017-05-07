#include "platform/i_platform.h"
#include "core/collision_store.h"
#include "core/empty_collision_model.h"
#include "core/box_collision_model.h"

void CollisionStore::SetCollisionModel( CollisionClass::Type T1, CollisionClass::Type T2, Elem_t Model )
{
    size_t Id = MapTypes( T1, T2 );
    mModels[Id] = Model;
    Id = MapTypes( T2, T1 );
    mModels[Id] = Model;
}

CollisionModel const& CollisionStore::GetCollisionModel( CollisionClass::Type T1, CollisionClass::Type T2 ) const
{
    return *( mModels.at( MapTypes( T1, T2 ) ) );
}

CollisionStore::CollisionStore()
{
    Elem_t Empty( new EmptyCollisionModel );
    size_t Size = MapTypes( CollisionClass::Num_Classes, CollisionClass::Num_Classes );
    mModels.resize( Size, Empty );
    Elem_t Box( new BoxCollisionModel );
    SetCollisionModel( CollisionClass::Creep, CollisionClass::Projectile, Box );
    SetCollisionModel( CollisionClass::Player, CollisionClass::Projectile, Box );
    SetCollisionModel( CollisionClass::Wall, CollisionClass::Projectile, Box );
    SetCollisionModel( CollisionClass::Wall, CollisionClass::Creep, Box );
    SetCollisionModel( CollisionClass::Wall, CollisionClass::Player, Box );
    SetCollisionModel( CollisionClass::Water, CollisionClass::Creep, Box );
    SetCollisionModel( CollisionClass::Water, CollisionClass::Player, Box );
    SetCollisionModel( CollisionClass::Player, CollisionClass::Pickup, Box );
    SetCollisionModel( CollisionClass::Creep, CollisionClass::AoeDamage, Box );
    SetCollisionModel( CollisionClass::Player, CollisionClass::AoeDamage, Box );
    SetCollisionModel( CollisionClass::Player, CollisionClass::Flag, Box );
    SetCollisionModel( CollisionClass::Flag, CollisionClass::Flag, Box );
}

size_t CollisionStore::MapTypes( CollisionClass::Type T1, CollisionClass::Type T2 )
{
    return size_t( T1 ) * CollisionClass::Num_Classes + T2;
}
