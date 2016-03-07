#ifndef INCLUDED_ENGINE_AOE_COLLISION_SUB_SYSTEM_H
#define INCLUDED_ENGINE_AOE_COLLISION_SUB_SYSTEM_H

#include "engine/collisions/collision_sub_system.h"

namespace engine {

class AoeCollisionSubSystem : public CollisionSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( AoeCollisionSubSystem )
    AoeCollisionSubSystem();
protected:
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
    virtual void ClipScene( Actor& actor );
    virtual void Collide( Actor& actor, Actor& other );
};

} // namespace engine

#endif//INCLUDED_ENGINE_AOE_COLLISION_SUB_SYSTEM_H

