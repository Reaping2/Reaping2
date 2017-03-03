#include "core/map/ctf_spawn_soldiers_map_element.h"
#include "../actor.h"
#include "../scene.h"

namespace map {
namespace ctf {

int32_t CtfSpawnSoldiersMapElement::SpawnNodeId()
{
    static int32_t id = AutoId( "spawn" );
    return id;
}

CtfSpawnSoldiersMapElement::CtfSpawnSoldiersMapElement( int32_t Id )
    : MapElement( Id )
    , BaseInput()
{
}

void CtfSpawnSoldiersMapElement::Load( Json::Value const& setters )
{
    MapElement::Load( setters );
    AddInputNodeId( SpawnNodeId() );
}

} // namespace ctf
} // namespace map
