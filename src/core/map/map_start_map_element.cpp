#include "core/map/map_start_map_element.h"
#include "platform/auto_id.h"

namespace map {

const int MapStartMapElement::StartNodeId=AutoId("start");

MapStartMapElement::MapStartMapElement()
    : MapElement()
    , BaseOutput()
{
    AddOutputNodeId(StartNodeId);
}

void MapStartMapElement::Load(Json::Value& setters)
{
    MapElement::Load(setters);
}


} // namespace map
