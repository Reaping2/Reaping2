#ifndef INCLUDED_MAP_POSSIBLE_ROOMS_H
#define INCLUDED_MAP_POSSIBLE_ROOMS_H

#include "room_repo.h"

namespace map {

class PossibleRooms 
{
public:
    PossibleRooms();
    void Load( Json::Value& possibleRooms );
    void ShuffleRoomIds();
    typedef std::vector<int32_t> PossibleRoomIds_t;
    PossibleRoomIds_t const& GetRoomIds();
    PossibleRoomIds_t GetRoomIdsByProperty( RoomProperty::Type prop, bool shuffled = true );
private:
    PossibleRoomIds_t mRoomIds;
    RoomRepo& mRoomRepo;
    void AddPossibleRoom( int32_t roomId, int32_t possibility );
};

} // namespace map

#endif//INCLUDED_MAP_POSSIBLE_ROOMS_H
