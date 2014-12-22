#ifndef INCLUDED_ENGINE_COLLISION_SYSTEM_H
#define INCLUDED_ENGINE_COLLISION_SYSTEM_H

#include "core\collision_store.h"
#include "core\grid.h"
#include "core\scene.h"
#include "engine\system.h"

namespace engine {

class CollisionSystem : public System
{
    Grid mCollisionGrid;
    CollisionStore& mCollisionStore;
    Scene& mScene;
public:
    CollisionSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
};

} // namespace engine

#endif//INCLUDED_ENGINE_COLLISION_SYSTEM_H
