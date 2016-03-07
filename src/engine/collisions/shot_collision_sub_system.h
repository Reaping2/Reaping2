#ifndef INCLUDED_ENGINE_SHOT_COLLISION_SUB_SYSTEM_H
#define INCLUDED_ENGINE_SHOT_COLLISION_SUB_SYSTEM_H

#include "core/scene.h"
#include "engine/collisions/collision_sub_system.h"
#include "core/shot_collision_component.h"

namespace engine {

class ShotCollisionSubSystem : public CollisionSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( ShotCollisionSubSystem )
    ShotCollisionSubSystem();
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
    virtual void ClipScene( Actor& actor );
    virtual void Collide( Actor& actor, Actor& other );
private:
    void TakeDamage( Actor& actor, Actor& target, Opt<ShotCollisionComponent> shotCC );

    void KillShot( Actor& actor, Actor& target, Opt<ShotCollisionComponent> shotCC );

};

} // namespace engine

#endif//INCLUDED_ENGINE_SHOT_COLLISION_SUB_SYSTEM_H
