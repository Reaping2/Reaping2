#ifndef INCLUDED_MAP_MAP_ELEMENT_REMOVED_EVENT_H
#define INCLUDED_MAP_MAP_ELEMENT_REMOVED_EVENT_H

#include "platform/event.h"
#include "map_element.h"
#include "../opt.h"

namespace map {

struct MapElementRemovedEvent : public platform::Event
{
    Opt<MapElement> mMapElement;
    MapElementRemovedEvent(Opt<MapElement> mapElement)
        :mMapElement(mapElement){}
};

} // namespace map

#endif//INCLUDED_MAP_MAP_ELEMENT_REMOVED_EVENT_H

//command:  "classgenerator.exe" -g "event" -n "map" -c "map_element_removed_event" -m "Opt<MapElement>-mapElement"
