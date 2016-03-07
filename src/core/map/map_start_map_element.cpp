#include "core/map/map_start_map_element.h"
#include "platform/auto_id.h"

namespace map {

int32_t MapStartMapElement::StartNodeId()
{
    static int32_t id = AutoId( "start" );
    return id;
}

MapStartMapElement::MapStartMapElement( int32_t Id )
    : MapElement( Id )
    , BaseOutput()
{
    AddOutputNodeId( StartNodeId() );
}

void MapStartMapElement::Load( Json::Value& setters )
{
    MapElement::Load( setters );
}


} // namespace map
