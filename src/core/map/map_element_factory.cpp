#include "platform/i_platform.h"
#include "map_element_factory.h"
#include "link_map_element.h"
#include "map_start_map_element.h"
#include "spawn_soldiers_map_element.h"
#include "soldier_spawn_point_map_element.h"

using platform::AutoId;
namespace map {

MapElementFactory::MapElementFactory()
{
    Bind( AutoId("default"), &CreateMapElement<DefaultMapElement> );
    SetDefault( AutoId("default") );

    Bind( AutoId("link"), &CreateMapElement<LinkMapElement> );
    Bind( AutoId("map_start"), &CreateMapElement<MapStartMapElement>);
    Bind( AutoId("spawn_soldiers"), &CreateMapElement<SpawnSoldiersMapElement>);
    Bind( AutoId("soldier_spawn_point"), &CreateMapElement<SoldierSpawnPointMapElement>);

}

} // namespace map

