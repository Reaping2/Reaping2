#include "core/map/map_start_map_element.h"
#include "platform/auto_id.h"

namespace map {

const int MapStartMapElement::StartNodeId=AutoId("start");

MapStartMapElement::MapStartMapElement(int32_t Id)
    : MapElement(Id)
    , BaseOutput()
{
    AddOutputNodeId(StartNodeId);
}

void MapStartMapElement::Load(Json::Value& setters)
{
    MapElement::Load(setters);
}


} // namespace map
