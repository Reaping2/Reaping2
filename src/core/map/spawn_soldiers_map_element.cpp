#include "core/map/spawn_soldiers_map_element.h"

namespace map {

const int SpawnSoldiersMapElement::SpawnNodeId=AutoId("spawn");

SpawnSoldiersMapElement::SpawnSoldiersMapElement(int32_t Id)
    : MapElement(Id)
    , BaseInput()
{
}

void SpawnSoldiersMapElement::Load(Json::Value& setters)
{
    MapElement::Load(setters);
    AddInputNodeId(SpawnNodeId);
}

} // namespace map
