#ifndef INCLUDED_ENGINE_LISTEN_CHILD_DEATH_SYSTEM_H
#define INCLUDED_ENGINE_LISTEN_CHILD_DEATH_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "removed_actors_system.h"

namespace engine {

class ListenChildDeathSystem : public System
{
public:
    DEFINE_SYSTEM_BASE( ListenChildDeathSystem )
    ListenChildDeathSystem();
protected:
    Opt<RemovedActorsSystem> mRemovedActorsSystem;
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_LISTEN_CHILD_DEATH_SYSTEM_H


