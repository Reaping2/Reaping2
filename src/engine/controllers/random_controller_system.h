#ifndef INCLUDED_RANDOM_CONTROLLER_SYSTEM_H
#define INCLUDED_RANDOM_CONTROLLER_SYSTEM_H

#include "core/scene.h"
#include "engine/sub_system.h"

namespace engine {

class RandomControllerSubSystem : public SubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(RandomControllerSubSystem)
    RandomControllerSubSystem();
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_RANDOM_CONTROLLER_SYSTEM_H
