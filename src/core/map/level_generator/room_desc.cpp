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
    return mPossibleProperties;
}

map::Cell& RoomDesc::GetCell( int32_t x, int32_t y )
{
    return mCells[y][x];
}


map::Cell const& RoomDesc::GetCell( int32_t x, int32_t y ) const
{
    return mCells[y][x];
}

void RoomDesc::ClearAllCellEntrances()
{
    for (auto& row : mCells)
    {
        for (auto& cell : row)
        {
            cell.mPossibleEntrances.clear();
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
