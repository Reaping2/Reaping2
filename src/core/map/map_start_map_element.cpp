#include "core/map/map_start_map_element.h"
#include "platform/auto_id.h"

namespace map {


int32_t MapStartMapElement::StartInputNodeId()
{
    static int32_t id = AutoId( "start" );
    return id;
}

int32_t MapStartMapElement::StartOutputNodeId()
{
    static int32_t id = AutoId( "start" );
    return id;
}


int32_t MapStartMapElement::PreActorsSpawnedInputNodeId()
{
    static int32_t id = AutoId( "pre_actors_spawned" );
    return id;
}


int32_t MapStartMapElement::PreActorsSpawnedOutputNodeId()
{
    static int32_t id = AutoId( "pre_actors_spawned" );
    return id;
}

int32_t MapStartMapElement::ActorsSpawnedInputNodeId()
{
    static int32_t id = AutoId( "actors_spawned" );
    return id;
}


int32_t MapStartMapElement::ActorsSpawnedOutputNodeId()
{
    static int32_t id = AutoId( "actors_spawned" );
    return id;
}

int32_t MapStartMapElement::ReadyInputNodeId()
{
    static int32_t id = AutoId( "ready" );
    return id;
}

int32_t MapStartMapElement::ReadyOutputNodeId()
{
    static int32_t id = AutoId( "ready" );
    return id;
}

MapStartMapElement::MapStartMapElement( int32_t Id )
    : MapElement( Id )
    , BaseOutput()
    , BaseInput()
{
    AddOutputNodeId( StartOutputNodeId() );
    AddOutputNodeId( PreActorsSpawnedOutputNodeId() );
    AddOutputNodeId( ActorsSpawnedOutputNodeId() );
    AddOutputNodeId( ReadyOutputNodeId() );
}

void MapStartMapElement::Load( Json::Value const& setters )
{
    MapElement::Load( setters );
    AddInputNodeId( StartInputNodeId() );
    AddInputNodeId( PreActorsSpawnedInputNodeId() );
    AddInputNodeId( ActorsSpawnedInputNodeId() );
    AddInputNodeId( ReadyInputNodeId() );
}

} // namespace map
