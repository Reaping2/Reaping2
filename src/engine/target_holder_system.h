#ifndef INCLUDED_ENGINE_TARGET_HOLDER_SYSTEM_H
#define INCLUDED_ENGINE_TARGET_HOLDER_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/actor_event.h"
#include "removed_actors_system.h"

namespace engine {

class TargetHolderSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(TargetHolderSystem)
    TargetHolderSystem();
protected:
    Opt<RemovedActorsSystem> mRemovedActorsSystem;
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_TARGET_HOLDER_SYSTEM_H

