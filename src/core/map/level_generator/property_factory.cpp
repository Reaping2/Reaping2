#include "platform/i_platform.h"
#include "property_factory.h"
#include "cell_entrance_property.h"
#include "room_start_property.h"
#include "room_end_property.h"
#include "spawn_property.h"

using platform::AutoId;
namespace map {

PropertyFactory::PropertyFactory()
{
    Bind( AutoId("default"), &CreateIProperty<DefaultProperty> );
    SetDefault( AutoId("default") );
    Bind( AutoId( "cell_entrance" ), &CreateIProperty<CellEntranceProperty> );
    Bind( AutoId( "room_start" ), &CreateIProperty<RoomStartProperty> );
    Bind( AutoId( "room_end" ), &CreateIProperty<RoomEndProperty> );
    Bind( AutoId( "spawn" ), &CreateIProperty<SpawnProperty> );
}

} // namespace map

