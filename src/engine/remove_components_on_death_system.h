#ifndef INCLUDED_ENGINE_REMOVE_COMPONENTS_ON_DEATH_SYSTEM_H
#define INCLUDED_ENGINE_REMOVE_COMPONENTS_ON_DEATH_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class RemoveComponentsOnDeathSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(RemoveComponentsOnDeathSystem)
    RemoveComponentsOnDeathSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_REMOVE_COMPONENTS_ON_DEATH_SYSTEM_H

//command:  "classgenerator" -g "system" -c "remove_components_on_death_system" -t "remove_components_on_death"
