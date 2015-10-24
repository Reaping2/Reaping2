#ifndef INCLUDED_ENGINE_FLAG_COLLISION_SUB_SYSTEM_H
#define INCLUDED_ENGINE_FLAG_COLLISION_SUB_SYSTEM_H

#include "engine/collisions/collision_sub_system.h"

namespace engine {
namespace ctf {

class FlagCollisionSubSystem : public CollisionSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(FlagCollisionSubSystem)
    FlagCollisionSubSystem();
protected:
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
    virtual void ClipScene( Actor& actor );
};

} // namespace ctf
} // namespace engine

#endif//INCLUDED_ENGINE_FLAG_COLLISION_SUB_SYSTEM_H

//command:  "classgenerator.exe" -g "collision_sub_system" -c "flag_collision_sub_system" -t "flag"
