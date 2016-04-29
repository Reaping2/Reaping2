#include "platform/i_platform.h"
#include "room_repo.h"
#include "simple_room1.h"

using platform::AutoId;

namespace map {

DefaultRoom const RoomRepo::mDefault = DefaultRoom();

RoomRepo::RoomRepo()
    : Repository<IRoom>(mDefault)
{
    int32_t id = AutoId( "simple_room1" );
    mElements.insert( id, new SimpleRoom1( id ) );
}

DefaultRoom::DefaultRoom()
    : IRoom( -1 )
{
}

void DefaultRoom::Generate( RoomDesc& roomDesc, int32_t x, int32_t y )
{

}

} // namespace map

