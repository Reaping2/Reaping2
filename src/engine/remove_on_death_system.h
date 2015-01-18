#ifndef INCLUDED_ENGINE_REMOVE_ON_DEATH_SYSTEM_H
#define INCLUDED_ENGINE_REMOVE_ON_DEATH_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class RemoveOnDeathSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(RemoveOnDeathSystem)
    RemoveOnDeathSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_REMOVE_ON_DEATH_SYSTEM_H
