#ifndef INCLUDED_MAP_POSSIBLE_ROOMS_H
#define INCLUDED_MAP_POSSIBLE_ROOMS_H

#include "room_repo.h"

namespace map {

struct PossibleRoom 
{
    int32_t mRoomId = -1;
    bool mIsBase = true;
    PossibleRoom( int32_t roomId, bool isBase )
        : mRoomId( roomId ), mIsBase( isBase ) {}
    PossibleRoom()=default;
    bool operator<( PossibleRoom const& other ) const;
};
class PossibleRooms 
{
public:
    PossibleRooms();
    void Load( Json::Value const& possibleRooms );
    void ShuffleRoomIds();
    typedef std::vector<PossibleRoom> PossibleRoomIds_t;
    PossibleRoomIds_t const& GetRoomIds()  const;
    PossibleRoomIds_t GetRoomIdsByProperty( RoomProperty::Type prop, bool shuffled = true ) const;
    PossibleRoomIds_t GetRoomIdsFiltered( RoomDesc const& roomDesc, int32_t flags, bool shuffled = true ) const;
    bool IsReplaceable( int32_t roomId ) const;
private:
    // depending on room weight there can be more than one roomId of the same Id.
    PossibleRoomIds_t mRoomIds;
    // unique (and shuffled) ids
    PossibleRoomIds_t mUniqueRoomIds;
    // don't forget to update mUniqueRoomIds!
    void AddPossibleRoom( int32_t roomId, int32_t possibility, bool isBase );
    PossibleRoomIds_t MakeRoomeIdsUnique(PossibleRoomIds_t const& roomIds) const;
};

} // namespace map

#endif//INCLUDED_MAP_POSSIBLE_ROOMS_H
