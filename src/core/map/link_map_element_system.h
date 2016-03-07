#ifndef INCLUDED_MAP_LINK_MAP_ELEMENT_SYSTEM_H
#define INCLUDED_MAP_LINK_MAP_ELEMENT_SYSTEM_H

#include "core/map_loaded_event.h"
#include "map_system.h"
#include "map_element_system.h"

namespace map {

class LinkMapElementSystem : public MapElementSystem
{
public:
    DEFINE_SYSTEM_BASE( LinkMapElementSystem )
    LinkMapElementSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    AutoReg mOnMapLoaded;
    void OnMapLoaded( core::MapLoadedEvent const& Evt );
};

} // namespace map

#endif//INCLUDED_MAP_LINK_MAP_ELEMENT_SYSTEM_H
