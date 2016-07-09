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

int32_t RoomDesc::GetCellSize() const
{
    return mCellSize;
}

RoomDesc::Properties_t const& RoomDesc::GetProperties() const
{
    return mPossibleProperties;
}


void RoomDesc::SetProperties( Properties_t const& properties )
{
    mPossibleProperties = properties;
}


bool RoomDesc::HasProperty( Property prop ) const
{
    return mPossibleProperties.find( prop ) != mPossibleProperties.end();
}

void RoomDesc::ClearProperties()
{
    mPossibleProperties.clear();
}


void RoomDesc::AddProperty( Property prop )
{
    mPossibleProperties.insert( prop );
}

map::Cell& RoomDesc::GetCell( int32_t x, int32_t y )
{
    return mCells[y][x];
}


map::Cell const& RoomDesc::GetCell( int32_t x, int32_t y ) const
{
    return mCells[y][x];
}


map::Cell& RoomDesc::GetCell( glm::vec2 pos )
{
    return mCells[pos.y][pos.x];
}


map::Cell const& RoomDesc::GetCell( glm::vec2 pos ) const
{
    return mCells[pos.y][pos.x];
}

bool RoomDesc::IsFilled( int32_t x, int32_t y ) const
{
    return GetCell( x, y ).mFilled;
}


bool RoomDesc::IsFilled( glm::vec2 pos ) const
{
    return GetCell( pos ).mFilled;
}

void RoomDesc::ClearCellEntrances()
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


void Cell::AddEntrance( Entrance const& entrance )
{
    mPossibleEntrances.insert( entrance );
}


void Cell::SetEntrances( Entrances_t const& entrances )
{
    mPossibleEntrances = entrances;
}


bool Cell::HasEntrance( Entrance const& entrance ) const
{
    return mPossibleEntrances.find( entrance ) != mPossibleEntrances.end();
}


Cell::Entrances_t const& Cell::GetEntrances() const
{
    return mPossibleEntrances;
}

void Cell::SetFilled( bool filled )
{
    mFilled = filled;
}


bool Cell::IsFilled()
{
    return mFilled;
}

} // namespace map
