#ifndef INCLUDED_ENGINE_WALL_COLLISION_SUB_SYSTEM_H
#define INCLUDED_ENGINE_WALL_COLLISION_SUB_SYSTEM_H

#include "core/scene.h"
#include "engine/collisions/collision_sub_system.h"

namespace engine {

class WallCollisionSubSystem : public CollisionSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(WallCollisionSubSystem)
    WallCollisionSubSystem();
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
    virtual void Collide(Actor& actor, Actor& other);
};

} // namespace engine

#endif//INCLUDED_ENGINE_WALL_COLLISION_SUB_SYSTEM_H
