#ifndef INCLUDED_MAP_MAP_START_MAP_ELEMENT_SYSTEM_H
#define INCLUDED_MAP_MAP_START_MAP_ELEMENT_SYSTEM_H

#include "core/map/map_element_system.h"
#include "core/map_start_event.h"

namespace map {

class MapStartMapElementSystem : public MapElementSystem
{
public:
    DEFINE_SYSTEM_BASE( MapStartMapElementSystem )
    MapStartMapElementSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    AutoReg mOnMapStart;
    void OnMapStart( core::MapStartEvent const& Evt );
};

} // namespace map

#endif//INCLUDED_MAP_MAP_START_MAP_ELEMENT_SYSTEM_H


