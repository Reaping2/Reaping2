#ifndef INCLUDED_ENGINE_CONTROLLERS_TARGET_PLAYER_CONTROLLER_SYSTEM_H
#define INCLUDED_ENGINE_CONTROLLERS_TARGET_PLAYER_CONTROLLER_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class TargetPlayerControllerSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(TargetPlayerControllerSystem)
    TargetPlayerControllerSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_CONTROLLERS_TARGET_PLAYER_CONTROLLER_SYSTEM_H
