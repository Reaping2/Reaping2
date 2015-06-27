#include "platform/i_platform.h"
#include "map_element_factory.h"
#include "link_map_element.h"

using platform::AutoId;
namespace map {

MapElementFactory::MapElementFactory()
{
    Bind( AutoId("default"), &CreateMapElement<DefaultMapElement> );
    SetDefault( AutoId("default") );

    Bind( AutoId("link"), &CreateMapElement<LinkMapElement> );

}

} // namespace map

