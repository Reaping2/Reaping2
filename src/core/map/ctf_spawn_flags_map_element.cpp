#include "core/map/ctf_spawn_flags_map_element.h"

namespace map {
namespace ctf {

int32_t CtfSpawnFlagsMapElement::SpawnNodeId()
{
    static int32_t id = AutoId( "spawn" );
    return id;
}

CtfSpawnFlagsMapElement::CtfSpawnFlagsMapElement( int32_t Id )
    : MapElement( Id )
    , BaseInput()
{
}

void CtfSpawnFlagsMapElement::Load( Json::Value const& setters )
{
    MapElement::Load( setters );
    AddInputNodeId( SpawnNodeId() );
}

} // namespace ctf
} // namespace map
