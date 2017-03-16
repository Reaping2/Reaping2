#ifndef INCLUDED_ENGINE_WAYPOINT_SYSTEM_H
#define INCLUDED_ENGINE_WAYPOINT_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/program_state.h"
#include <vector>
#include <map>
#include "waypoint_event.h"

namespace engine {

class WaypointSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(WaypointSystem)
    WaypointSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    int32_t mNextWaypointCounter = 0;
    using WaypointNames_t = std::vector<std::string>;
    WaypointNames_t mWaypointNames;
    using WaypointGUIDs_t = std::map<std::string, int32_t>;
    WaypointGUIDs_t mWaypointGUIDs;
    using PlayerToWaypoint_t = std::map<int32_t, int32_t>;
    PlayerToWaypoint_t mPlayerToWaypoint;
    ModelValue mWaypointModel;
    ModelValue mWaypointNamesModel;
    ModelValue mWaypointSelectModel;
    void OnWaypointSelect( std::string waypointName );
    Scene& mScene = Scene::Get();
    core::ProgramState& mProgramState = core::ProgramState::Get();
    AutoReg mOnMapStart;
    void OnMapStart( core::MapStartEvent const& Evt );
    AutoReg mOnWaypointChanged;
    void OnWaypointChanged( WaypointChangedEvent const& Evt );
};

} // namespace engine

#endif//INCLUDED_ENGINE_WAYPOINT_SYSTEM_H


//command:  "classgenerator.exe" -g "component" -c "waypoint_component"
