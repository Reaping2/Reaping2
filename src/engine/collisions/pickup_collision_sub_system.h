#ifndef INCLUDED_ENGINE_COLLISIONS_PICKUP_COLLISION_SUB_SYSTEM_H
#define INCLUDED_ENGINE_COLLISIONS_PICKUP_COLLISION_SUB_SYSTEM_H

#include "core/scene.h"
#include "engine/collisions/collision_sub_system.h"

namespace engine {

class PickupCollisionSubSystem : public CollisionSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(PickupCollisionSubSystem)
    PickupCollisionSubSystem();
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
};

} // namespace engine

#endif//INCLUDED_ENGINE_COLLISIONS_PICKUP_COLLISION_SUB_SYSTEM_H
