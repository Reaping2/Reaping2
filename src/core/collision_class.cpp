#include "core/collision_class.h"
#include "platform/auto_id.h"

using platform::AutoId;

CollisionClass::CollisionClass()
{
    mIdToCollClassMap[AutoId( "no_collision" )] = CollisionClass::No_Collision;
    mIdToCollClassMap[AutoId( "projectile" )] = CollisionClass::Projectile;
    mIdToCollClassMap[AutoId( "aoe" )] = CollisionClass::AoeDamage;

    mIdToCollClassMap[AutoId( "creep" )] = CollisionClass::Creep;
    mIdToCollClassMap[AutoId( "mine" )] = CollisionClass::Mine;
    mIdToCollClassMap[AutoId( "player" )] = CollisionClass::Player;
    mIdToCollClassMap[AutoId( "wall" )] = CollisionClass::Wall;
    mIdToCollClassMap[AutoId( "water" )] = CollisionClass::Water;
    mIdToCollClassMap[AutoId( "pickup" )] = CollisionClass::Pickup;
    mIdToCollClassMap[AutoId( "flag" )] = CollisionClass::Flag;
    mIdToCollClassMap[AutoId( "activatable" )] = CollisionClass::Activatable;
}

CollisionClass::Type CollisionClass::operator()( int32_t Id ) const
{
    IdToCollClassMap_t::const_iterator i = mIdToCollClassMap.find( Id );
    BOOST_ASSERT( i != mIdToCollClassMap.end() );
    return ( i != mIdToCollClassMap.end() ) ? i->second : CollisionClass::No_Collision;
}
