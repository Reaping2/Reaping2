#ifndef INCLUDED_CORE_COLLISION_CLASS_H
#define INCLUDED_CORE_COLLISION_CLASS_H
#include "platform/singleton.h"
#include <map>

class CollisionClass : public platform::Singleton<CollisionClass>
{
protected:
    friend class platform::Singleton<CollisionClass>;
    CollisionClass();
public:
    enum Type : int32_t
    {
        No_Collision = 0,
        Projectile,
        AoeDamage,
        Creep,
        Mine,
        Player,
        Wall,
        Water,
        Pickup,
        Flag,
        Activatable,
        Num_Classes,
    };
    CollisionClass::Type operator()( int32_t Id ) const;
private:
    typedef std::map<int32_t, CollisionClass::Type> IdToCollClassMap_t;
    IdToCollClassMap_t mIdToCollClassMap;
};


#endif//INCLUDED_CORE_COLLISION_CLASS_H
