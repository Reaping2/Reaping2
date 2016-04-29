#include "room_desc.h"
#include "i_room.h"

namespace map {

RoomDesc::RoomDesc()
{
}

void RoomDesc::SetCellCount( int32_t cellCount )
{
    mCellCount = cellCount;
    mCells.clear();
    mCells.resize( mCellCount );
    for (auto& row : mCells)
    {
        row.resize( mCellCount );
    }
}

int32_t RoomDesc::GetCellCount() const
{
    return mCellCount;
}

void RoomDesc::SetCellSize( int32_t cellSize )
{
    mCellSize = cellSize;
}

int32_t RoomDesc::GetCellSize()
{
    return mCellSize;
}

map::RoomDesc::Properties_t& RoomDesc::GetProperties()
{
    return mProperties;
}

map::RoomDesc::CellMatrix_t& RoomDesc::GetCells()
{
    return mCells;
}


void RoomDesc::ClearAllCellentrances()
{
    for (auto& row : mCells)
    {
        for (auto& cell : row)
        {
            cell.mEntrances.clear();
        }
    }
}

Opt<IRoom> RoomDesc::GetRoom()
{
    return mRoom;
}


void RoomDesc::SetRoom( Opt<IRoom> room )
{
    mRoom = room;
}

} // namespace map
