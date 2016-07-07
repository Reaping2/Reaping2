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
    for (int32_t y = 0; y < mCellCount; ++y)
    {
        mCells[y].resize( mCellCount );
        for (int32_t x = 0; x < mCellCount; ++x)
        {
            mCells[y][x].mDescCoord = glm::vec2( x, y );
        }
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


map::RoomDesc::Properties_t const& RoomDesc::GetProperties() const
{
    return mPossibleProperties;
}

void RoomDesc::ClearProperties()
{
    mPossibleProperties.clear();
}


void RoomDesc::AddProperty( Property prop )
{
    mPossibleProperties.insert( prop );
}


void RoomDesc::AddCellEntrance( glm::vec2 pos, Cell::Entrance entrance )
{
    GetCell( pos.x, pos.y ).mPossibleEntrances.insert( entrance );
}

map::Cell& RoomDesc::GetCell( int32_t x, int32_t y )
{
    return mCells[y][x];
}


map::Cell const& RoomDesc::GetCell( int32_t x, int32_t y ) const
{
    return mCells[y][x];
}


bool RoomDesc::IsFilled( int32_t x, int32_t y ) const
{
    return GetCell( x, y ).mFilled;
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
