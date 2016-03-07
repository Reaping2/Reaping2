#ifndef INCLUDED_ENGINE_COLLISIONS_COLLISION_SUB_SYSTEM_H
#define INCLUDED_ENGINE_COLLISIONS_COLLISION_SUB_SYSTEM_H

#include "core/scene.h"
#include "engine/sub_system.h"

namespace engine {

class CollisionSubSystem : public SubSystem
{
public:
    CollisionSubSystem();
    virtual void ClipScene( Actor& actor );
    virtual void Collide( Actor& actor, Actor& other );
protected:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_COLLISIONS_COLLISION_SUB_SYSTEM_H
