#include "i_room.h"

namespace map {

IRoom::IRoom( int32_t Id )
    : mId(Id)
    , mScene(Scene::Get())
{

}

IRoom::~IRoom()
{

}

RoomDesc IRoom::GetRoomDesc()const
{
    return mRoomDesc;
}

} // namespace map
