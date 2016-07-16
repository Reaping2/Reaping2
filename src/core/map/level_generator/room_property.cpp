#include "core/map/level_generator/room_property.h"
#include "platform/auto_id.h"

using platform::AutoId;

namespace map {

RoomProperty::RoomProperty()
{
    mIdToRoomPropertyMap.insert(IdToRoomPropertyMap_t::value_type(AutoId("start"),RoomProperty::Start));
    mIdToRoomPropertyMap.insert(IdToRoomPropertyMap_t::value_type(AutoId("end"),RoomProperty::End));
    mIdToRoomPropertyMap.insert(IdToRoomPropertyMap_t::value_type(AutoId("key"),RoomProperty::Key));
}

RoomProperty::Type RoomProperty::operator()( int32_t Id ) const
{
    IdToRoomPropertyMap_t::left_const_iterator i=mIdToRoomPropertyMap.left.find(Id);
    BOOST_ASSERT(i!=mIdToRoomPropertyMap.left.end());
    return (i!=mIdToRoomPropertyMap.left.end())?i->second:RoomProperty::Start;
}

int32_t RoomProperty::operator()( Type type ) const
{
    IdToRoomPropertyMap_t::right_const_iterator i=mIdToRoomPropertyMap.right.find(type);
    BOOST_ASSERT(i!=mIdToRoomPropertyMap.right.end());
    return (i!=mIdToRoomPropertyMap.right.end())?i->second:RoomProperty::Start;
}

} // namespace map

