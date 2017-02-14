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
    AddOutputNodeId( ReadyOutputNodeId() );
}

void MapStartMapElement::Load( Json::Value& setters )
{
    MapElement::Load( setters );
    AddInputNodeId( StartInputNodeId() );
    AddInputNodeId( ReadyInputNodeId() );
}

} // namespace map
