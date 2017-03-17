#ifndef INCLUDED_ENGINE_WAYPOINT_SYSTEM_H
#define INCLUDED_ENGINE_WAYPOINT_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/program_state.h"
#include <vector>
#include <map>
#include "waypoint_event.h"
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>

namespace engine {

struct WaypointsData
{
    int32_t mNextWaypointCounter = 0;
    using WaypointNames_t = std::vector<std::string>;
    WaypointNames_t mWaypointNames;
    using WaypointGUIDs_t = std::map<std::string, int32_t>;
    WaypointGUIDs_t mWaypointGUIDs;
    using PlayerToWaypoint_t = std::map<int32_t, int32_t>;
    PlayerToWaypoint_t mPlayerToWaypoint;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void engine::WaypointsData::serialize( Archive& ar, const unsigned int version )
{
    ar& mNextWaypointCounter;
    ar& mWaypointNames;
    ar& mWaypointGUIDs;
    ar& mPlayerToWaypoint;
}

class WaypointSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(WaypointSystem)
    WaypointSystem();
    void SetWaypointsData( WaypointsData const& waypointsData );
    WaypointsData& GetWaypointsData();
    static const std::string NextStage;
    static const std::string Resume;
    static const std::string WaypointName;
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    WaypointsData mWaypointsData;

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
    std::set<Actor*> GetCollidingPlayers( int32_t waypointGUID );
    int32_t GetAlivePlayerCount();
};

} // namespace engine

#endif//INCLUDED_ENGINE_WAYPOINT_SYSTEM_H


//command:  "classgenerator.exe" -g "component" -c "waypoint_component"
