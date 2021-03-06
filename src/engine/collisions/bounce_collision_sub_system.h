#ifndef INCLUDED_ENGINE_BOUNCE_COLLISION_SUB_SYSTEM_H
#define INCLUDED_ENGINE_BOUNCE_COLLISION_SUB_SYSTEM_H

#include "engine/collisions/collision_sub_system.h"
#include "shot_collision_sub_system.h"

namespace engine {

class BounceCollisionSubSystem : public CollisionSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( BounceCollisionSubSystem )
    BounceCollisionSubSystem();
protected:
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
    virtual void ClipScene( Actor& actor );
    virtual void Collide( Actor& actor, Actor& other );
    Opt<ShotCollisionSubSystem> mShotCollisionSubSystem;
};

} // namespace engine

#endif//INCLUDED_ENGINE_BOUNCE_COLLISION_SUB_SYSTEM_H
