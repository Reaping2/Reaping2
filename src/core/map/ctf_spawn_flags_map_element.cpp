#include "core/map/ctf_spawn_flags_map_element.h"

namespace map {
namespace ctf {

const int CtfSpawnFlagsMapElement::SpawnNodeId=AutoId("spawn");

CtfSpawnFlagsMapElement::CtfSpawnFlagsMapElement(int32_t Id)
    : MapElement(Id)
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
