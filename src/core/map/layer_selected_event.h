#ifndef INCLUDED_MAP_LAYER_SELECTED_EVENT_H
#define INCLUDED_MAP_LAYER_SELECTED_EVENT_H

#include "platform/event.h"

namespace map {

struct LayerSelectedEvent : public platform::Event
{
    std::string mLayerName;
    LayerSelectedEvent(std::string const& layerName)
        :mLayerName(layerName){}
};

} // namespace map

#endif//INCLUDED_MAP_LAYER_SELECTED_EVENT_H

//command:  "classgenerator.exe" -g "event" -n "map" -c "layer_selected_event" -m "std::string-layerName"
