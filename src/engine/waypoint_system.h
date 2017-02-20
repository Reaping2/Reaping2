#ifndef INCLUDED_ENGINE_WAYPOINT_SYSTEM_H
#define INCLUDED_ENGINE_WAYPOINT_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

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
    Scene& mScene;
    AutoReg mOnMapStart;
    void OnMapStart( core::MapStartEvent const& Evt );
};

} // namespace engine

#endif//INCLUDED_ENGINE_WAYPOINT_SYSTEM_H


//command:  "classgenerator.exe" -g "component" -c "waypoint_component"
