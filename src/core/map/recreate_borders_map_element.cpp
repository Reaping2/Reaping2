#include "core/map/recreate_borders_map_element.h"

namespace map {

RecreateBordersMapElement::RecreateBordersMapElement( int32_t Id )
    : MapElement( Id )
    , BaseInput()
{
}

void RecreateBordersMapElement::Load( Json::Value const& setters )
{
    MapElement::Load( setters );
    AddInputNodeId( RecreateNodeId() );
}


int32_t RecreateBordersMapElement::RecreateNodeId()
{
    static int32_t id = AutoId( "recreate" );
    return id;
}

} // namespace map
