#ifndef INCLUDED_RANDOM_CONTROLLER_SYSTEM_H
#define INCLUDED_RANDOM_CONTROLLER_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class RandomControllerSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(RandomControllerSystem)
    RandomControllerSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_RANDOM_CONTROLLER_SYSTEM_H
