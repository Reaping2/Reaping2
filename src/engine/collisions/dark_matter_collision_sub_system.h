#ifndef INCLUDED_ENGINE_DARK_MATTER_COLLISION_SUB_SYSTEM_H
#define INCLUDED_ENGINE_DARK_MATTER_COLLISION_SUB_SYSTEM_H

#include "engine/collisions/collision_sub_system.h"

namespace engine {

class DarkMatterCollisionSubSystem : public CollisionSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(DarkMatterCollisionSubSystem)
    DarkMatterCollisionSubSystem();
protected:
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
    virtual void Collide( Actor& actor, Actor& other );
    virtual void ClipScene( Actor& actor );
};

} // namespace engine

#endif//INCLUDED_ENGINE_DARK_MATTER_COLLISION_SUB_SYSTEM_H

//command:  "classgenerator" -g "collision_sub_system" -c "dark_matter_collision_sub_system" -t "dark_matter"
