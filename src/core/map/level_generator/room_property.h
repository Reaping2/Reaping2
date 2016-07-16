#ifndef INCLUDED_MAP_ROOM_PROPERTY_H
#define INCLUDED_MAP_ROOM_PROPERTY_H

#include "platform/singleton.h"
#include "boost/bimap.hpp"

namespace map {

class RoomProperty : public platform::Singleton<RoomProperty>
{
protected:
    friend class platform::Singleton<RoomProperty>;
    RoomProperty();
public:
    enum Type
    {
        Start=0,
        End,
        Key,
        Num_Classes
    };
    RoomProperty::Type operator()( int32_t Id ) const;
    int32_t operator()( RoomProperty::Type type ) const;
private:
    typedef boost::bimap<int32_t,RoomProperty::Type> IdToRoomPropertyMap_t;
    IdToRoomPropertyMap_t mIdToRoomPropertyMap;
};

} // namespace map

#endif//INCLUDED_MAP_ROOM_PROPERTY_H


//command:  "classgenerator.exe" -g "enum" -c "room_property" -n "map" -m "Start-start End-end Key-key"
