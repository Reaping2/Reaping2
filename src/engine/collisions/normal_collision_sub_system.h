#ifndef INCLUDED_ENGINE_NORMAL_COLLISION_SUB_SYSTEM_H
#define INCLUDED_ENGINE_NORMAL_COLLISION_SUB_SYSTEM_H

#include "core/scene.h"
#include "engine/collisions/collision_sub_system.h"

namespace engine {

class NormalCollisionSubSystem : public CollisionSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(NormalCollisionSubSystem)
    NormalCollisionSubSystem();
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
};

} // namespace engine

#endif//INCLUDED_ENGINE_NORMAL_COLLISION_SUB_SYSTEM_H
