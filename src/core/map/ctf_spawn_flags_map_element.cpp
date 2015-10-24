#include "core/map/ctf_spawn_flags_map_element.h"

namespace map {
namespace ctf {

const int CtfSpawnFlagsMapElement::SpawnNodeId=AutoId("spawn");

CtfSpawnFlagsMapElement::CtfSpawnFlagsMapElement()
    : MapElement()
    , BaseInput()
{
}

void CtfSpawnFlagsMapElement::Load(Json::Value& setters)
{
    MapElement::Load(setters);
    AddInputNodeId(SpawnNodeId);
}

} // namespace ctf
} // namespace map
