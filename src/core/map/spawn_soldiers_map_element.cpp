#include "core/map/spawn_soldiers_map_element.h"

namespace map {

int SpawnSoldiersMapElement::SpawnNodeId=AutoId("spawn");

SpawnSoldiersMapElement::SpawnSoldiersMapElement()
    : MapElement()
    , BaseInput()
    , mSpawnInputCounter(0)
{
}

void SpawnSoldiersMapElement::Load(Json::Value& setters)
{
    MapElement::Load(setters);
    AddInputNode(SpawnNodeId,this,&SpawnSoldiersMapElement::SpawnInput);
}

void SpawnSoldiersMapElement::SpawnInput(int32_t in)
{
    mSpawnInputCounter+=in;
}


} // namespace map
