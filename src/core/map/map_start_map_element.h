#ifndef INCLUDED_MAP_MAP_START_H
#define INCLUDED_MAP_MAP_START_H

#include "core/map/map_element.h"
#include "platform/i_platform.h"
#include "base_output.h"

namespace map {

class MapStartMapElement : public MapElement, public BaseOutput
{
public:
    DEFINE_MAP_ELEMENT_BASE(MapStartMapElement)
    MapStartMapElement(int32_t Id);
    void Load(Json::Value& setters);
    static const int32_t StartNodeId;
private:
};

} // namespace map

#endif//INCLUDED_MAP_MAP_START_H

