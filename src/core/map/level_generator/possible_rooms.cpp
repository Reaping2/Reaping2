#include "possible_rooms.h"
#include "random.h"

namespace map {

PossibleRooms::PossibleRooms()
{

}


void PossibleRooms::Load( Json::Value const& possibleRooms )
{
    mRoomIds.clear();
    if (possibleRooms.isArray())
    {
        for (auto& possibleRoom : possibleRooms)
        {
            std::string roomIdStr;
            int32_t chance;
            if (Json::GetStr( possibleRoom["room_id"], roomIdStr ) && Json::GetInt( possibleRoom["weight"], chance ))
            {
                int32_t isBase = 1;
                Json::GetInt( possibleRoom["base"], isBase );
                AddPossibleRoom( AutoId( roomIdStr ), chance, isBase==1 );
            }
        }
    }
    mUniqueRoomIds = MakeRoomeIdsUnique( mRoomIds );
}


void PossibleRooms::ShuffleRoomIds()
{
    std::shuffle( mRoomIds.begin(), mRoomIds.end(), mRand );
    mUniqueRoomIds = MakeRoomeIdsUnique( mRoomIds );
}


PossibleRooms::PossibleRoomIds_t const& PossibleRooms::GetRoomIds() const
{
    return mUniqueRoomIds;
}


PossibleRooms::PossibleRoomIds_t PossibleRooms::GetRoomIdsByProperty( RoomProperty::Type prop, bool shuffled /*= true */ ) const
{
    PossibleRoomIds_t r;
    PossibleRoomIds_t roomIds;
    if (shuffled)
    {
        roomIds = mRoomIds;
        std::shuffle( roomIds.begin(), roomIds.end(), mRand );
        roomIds = MakeRoomeIdsUnique( roomIds );
    }
    else
    {
        roomIds = mUniqueRoomIds;
    }

    static auto& mRoomRepo = RoomRepo::Get();
    for (auto&& roomId : roomIds)
    {
        if (mRoomRepo( roomId.mRoomId ).GetRoomDesc().HasProperty( prop ))
        {
            r.push_back( roomId );
        }
    }
    return r;
}


PossibleRooms::PossibleRoomIds_t PossibleRooms::GetRoomIdsFiltered( RoomDesc const& roomDesc, int32_t flags, bool shuffled /*= true*/ ) const
{
    PossibleRoomIds_t r;
    PossibleRoomIds_t roomIds;
    if (shuffled)
    {
        roomIds = mRoomIds;
        std::shuffle( roomIds.begin(), roomIds.end(), mRand );
        roomIds = MakeRoomeIdsUnique( roomIds );
    }
    else
    {
        roomIds = mUniqueRoomIds;
    }

    static auto& mRoomRepo = RoomRepo::Get();
    for (auto&& roomId : roomIds)
    {
        if (roomDesc.FitsInto(mRoomRepo( roomId.mRoomId ).GetRoomDesc(), flags))
        {
            r.push_back( roomId );
        }
    }

    return r;
}


bool PossibleRooms::IsReplaceable(int32_t roomId ) const
{
    auto foundSelf = std::find_if( mUniqueRoomIds.begin(), mUniqueRoomIds.end(), [&]( PossibleRoom const& room ) { return room.mRoomId == roomId; } );
    if (foundSelf != mUniqueRoomIds.end() && foundSelf->mIsBase)
    {
        return true;
    }
    static auto& mRoomRepo = RoomRepo::Get();
    auto const& roomDesc = mRoomRepo( roomId ).GetRoomDesc();
    auto found = std::find_if( mUniqueRoomIds.begin(), mUniqueRoomIds.end(), [&]( PossibleRoom const& room ) {
            return roomDesc.FitsInto( mRoomRepo( room.mRoomId ).GetRoomDesc(), RoomDesc::Layout ) 
                    && room.mIsBase; } );
    return found != mUniqueRoomIds.end();
}

void PossibleRooms::AddPossibleRoom( int32_t roomId, int32_t possibility, bool isBase )
{
    for (int i = 0; i < possibility; ++i)
    {
        mRoomIds.emplace_back(roomId, isBase);
    }
}


PossibleRooms::PossibleRoomIds_t PossibleRooms::MakeRoomeIdsUnique( PossibleRoomIds_t const& roomIds ) const
{
    PossibleRoomIds_t r;
    std::set<PossibleRoom> roomIdSet;
    for (auto&& roomId : roomIds)
    {
        auto ipair = roomIdSet.insert( roomId );
        if (ipair.second)
        {
            r.push_back( roomId );
        }
    }
    return r;
}


bool PossibleRoom::operator<( PossibleRoom const& other ) const
{
    return mRoomId < other.mRoomId;
}

} // namespace map

