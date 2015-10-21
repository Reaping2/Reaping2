#include "core/map/ctf_spawn_soldiers_map_element.h"

namespace map {
namespace ctf {

const int CtfSpawnSoldiersMapElement::SpawnNodeId=AutoId("spawn");

CtfSpawnSoldiersMapElement::CtfSpawnSoldiersMapElement()
    : MapElement()
    , BaseInput()
{
}

void CtfSpawnSoldiersMapElement::Load(Json::Value& setters)
{
    MapElement::Load(setters);
    AddInputNodeId(SpawnNodeId);
}

} // namespace ctf
} // namespace map
