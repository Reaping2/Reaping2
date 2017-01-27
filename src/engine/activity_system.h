#ifndef INCLUDED_ENGINE_ACTIVITY_SYSTEM_H
#define INCLUDED_ENGINE_ACTIVITY_SYSTEM_H

#include "core/activity_grid.h"
#include "core/scene.h"
#include "engine/system.h"
#include "core/perf_timer.h"
#include "core/actor_event.h"
#include <boost/container/flat_set.hpp>

namespace engine {

class ActivitySystem : public System
{
public:
    DEFINE_SYSTEM_BASE( ActivitySystem )
    ActivitySystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
    struct ActorOrderer {
        bool operator()( Actor const* a, Actor const* b ) const;
    };
    typedef boost::container::flat_set<Actor*,ActorOrderer> OrderedActors_t;
    OrderedActors_t const& GetActiveActors();
    glm::vec4 const& GetActiveRegion();
private:
    ActivityGrid mActivityGrid;
    OrderedActors_t mActiveActors;
    glm::vec4 mActiveRegion;
    bool mDirty = false;
    Scene& mScene;
    perf::Timer_t mPerfTimer;
    perf::Timer_t mUpdateTimer;
    AutoReg mOnActorEvent;
    void OnActorEvent( ActorEvent const& Evt );
};

} // namespace engine

#endif//INCLUDED_ENGINE_ACTIVITY_SYSTEM_H
