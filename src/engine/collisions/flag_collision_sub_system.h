#ifndef INCLUDED_ENGINE_FLAG_COLLISION_SUB_SYSTEM_H
#define INCLUDED_ENGINE_FLAG_COLLISION_SUB_SYSTEM_H

#include "engine/collisions/collision_sub_system.h"
#include "../attach_state_changed_event.h"

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
    virtual void Collide(Actor& actor, Actor& other);
    AutoReg mOnAttachStateChanged;
    void OnAttachStateChanged(engine::AttachStateChangedEvent const& Evt);
};

} // namespace ctf
} // namespace engine

#endif//INCLUDED_ENGINE_FLAG_COLLISION_SUB_SYSTEM_H

//command:  "classgenerator.exe" -g "collision_sub_system" -c "flag_collision_sub_system" -t "flag"
