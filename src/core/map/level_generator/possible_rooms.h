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
    PossibleRoomIds_t GetRoomIdsFiltered( RoomDesc const& roomDesc, int32_t flags, bool shuffled = true );
private:
    // depending on room weight there can be more than one roomId of the same Id.
    PossibleRoomIds_t mRoomIds;
    // unique (and shuffled) ids
    PossibleRoomIds_t mUniqueRoomIds;
    RoomRepo& mRoomRepo;
    // don't forget to update mUniqueRoomIds!
    void AddPossibleRoom( int32_t roomId, int32_t possibility );
    PossibleRoomIds_t MakeRoomeIdsUnique(PossibleRoomIds_t const& roomIds);
};

} // namespace map

#endif//INCLUDED_MAP_POSSIBLE_ROOMS_H
