#ifndef INCLUDED_ENGINE_CONTROLLERS_TARGET_PLAYER_CONTROLLER_SUB_SYSTEM_H
#define INCLUDED_ENGINE_CONTROLLERS_TARGET_PLAYER_CONTROLLER_SUB_SYSTEM_H

#include "core/scene.h"
#include "engine/sub_system.h"

namespace engine {

class TargetPlayerControllerSubSystem : public SubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(TargetPlayerControllerSubSystem)
    TargetPlayerControllerSubSystem();
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_CONTROLLERS_TARGET_PLAYER_CONTROLLER_SUB_SYSTEM_H
