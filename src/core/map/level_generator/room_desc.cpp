#include "room_desc.h"

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

void RoomDesc::SetProperties( Properties_t& properties )
{
    mProperties = properties;
}

map::RoomDesc::Properties_t const& RoomDesc::GetProperties() const
{
    return mProperties;
}

map::RoomDesc::CellMatrix_t& RoomDesc::GetCells()
{
    return mCells;
}

} // namespace map
