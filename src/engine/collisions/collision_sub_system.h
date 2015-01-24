#ifndef INCLUDED_ENGINE_COLLISIONS_COLLISION_SUB_SYSTEM_H
#define INCLUDED_ENGINE_COLLISIONS_COLLISION_SUB_SYSTEM_H

#include "core/scene.h"
#include "engine/sub_system.h"

namespace engine {

class CollisionSubSystem : public SubSystem
{
public:
    CollisionSubSystem();
    void SetOther(Actor* other);
    virtual void ClipScene(Actor& actor);
protected:
    Scene& mScene;
    Actor* mOther;
};

} // namespace engine

#endif//INCLUDED_ENGINE_COLLISIONS_COLLISION_SUB_SYSTEM_H
