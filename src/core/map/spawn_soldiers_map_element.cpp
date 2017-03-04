#include "core/map/spawn_soldiers_map_element.h"

namespace map {

int32_t SpawnSoldiersMapElement::SpawnNodeId()
{
    static int32_t id = AutoId( "spawn" );
    return id;
}

SpawnSoldiersMapElement::SpawnSoldiersMapElement( int32_t Id )
    : MapElement( Id )
    , BaseInput()
{
}

void SpawnSoldiersMapElement::Load( Json::Value const& setters )
{
    MapElement::Load( setters );
    AddInputNodeId( SpawnNodeId() );
}

} // namespace map
