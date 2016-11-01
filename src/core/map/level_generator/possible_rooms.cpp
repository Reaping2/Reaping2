#include "possible_rooms.h"
#include "random.h"

namespace map {

PossibleRooms::PossibleRooms()
    : mRoomRepo(RoomRepo::Get())
{

}


void PossibleRooms::Load( Json::Value& possibleRooms )
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
                AddPossibleRoom( AutoId( roomIdStr ), chance );
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


PossibleRooms::PossibleRoomIds_t const& PossibleRooms::GetRoomIds()
{
    return mUniqueRoomIds;
}


PossibleRooms::PossibleRoomIds_t PossibleRooms::GetRoomIdsByProperty( RoomProperty::Type prop, bool shuffled /*= true */ )
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

    for (auto roomId : roomIds)
    {
        if (mRoomRepo( roomId ).GetRoomDesc().HasProperty( prop ))
        {
            r.push_back( roomId );
        }
    }
    return r;
}


PossibleRooms::PossibleRoomIds_t PossibleRooms::GetRoomIdsFiltered( RoomDesc const& roomDesc, int32_t flags, bool shuffled /*= true*/ )
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

    for (auto roomId : roomIds)
    {
        if (roomDesc.FitsInto(mRoomRepo( roomId ).GetRoomDesc(), flags))
        {
            r.push_back( roomId );
        }
    }

    return r;
}

void PossibleRooms::AddPossibleRoom( int32_t roomId, int32_t possibility )
{
    for (int i = 0; i < possibility; ++i)
    {
        mRoomIds.push_back( roomId );
    }
}


PossibleRooms::PossibleRoomIds_t PossibleRooms::MakeRoomeIdsUnique( PossibleRoomIds_t const& roomIds )
{
    PossibleRoomIds_t r;
    std::set<int32_t> roomIdSet;
    for (auto roomId : roomIds)
    {
        auto ipair = roomIdSet.insert( roomId );
        if (ipair.second)
        {
            r.push_back( roomId );
        }
    }
    return r;
}

} // namespace map

