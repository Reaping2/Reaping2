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

}


void PossibleRooms::ShuffleRoomIds()
{
    std::shuffle( mRoomIds.begin(), mRoomIds.end(), mRand );
}


PossibleRooms::PossibleRoomIds_t const& PossibleRooms::GetRoomIds()
{
    return mRoomIds;
}


PossibleRooms::PossibleRoomIds_t PossibleRooms::GetRoomIdsByProperty( RoomProperty::Type prop, bool shuffled /*= true */ )
{
    PossibleRoomIds_t r;
    for (auto roomId : mRoomIds)
    {
        if (mRoomRepo( roomId ).GetRoomDesc().HasProperty( prop ))
        {
            r.push_back( roomId );
        }
    }
    if (shuffled)
    {
        std::shuffle( r.begin(), r.end(), mRand );
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

} // namespace map

