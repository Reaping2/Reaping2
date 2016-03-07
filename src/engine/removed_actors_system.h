#ifndef INCLUDED_ENGINE_REMOVED_ACTORS_SYSTEM_H
#define INCLUDED_ENGINE_REMOVED_ACTORS_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include <set>
#include "core/actor_event.h"

namespace engine {

class RemovedActorsSystem : public System
{
public:
    DEFINE_SYSTEM_BASE( RemovedActorsSystem )
    RemovedActorsSystem();
    typedef std::set<int32_t> RemovedActorsGUIDs_t;
    RemovedActorsGUIDs_t const& GetRemovedActors() const;
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    RemovedActorsGUIDs_t mRemovedActorGUIDs;
    AutoReg mOnActorEvent;
    void OnActorEvent( ActorEvent const& Evt );
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_REMOVED_ACTORS_SYSTEM_H
