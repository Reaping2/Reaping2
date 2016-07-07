#include "generator_data.h"

namespace map {

GeneratorData::GeneratorData()
{

}

GCell& GeneratorData::GetGCell( int32_t x, int32_t y )
{
    return mGCells[y][x];
}

GCell const& GeneratorData::GetGCell( int32_t x, int32_t y ) const
{
    return mGCells[y][x];
}

void GeneratorData::SetSize( int32_t x, int32_t y )
{
    mDimX = x;
    mDimY = y;
    mGCells.clear();
    mGCells.resize( mDimY );
    for (auto& row : mGCells)
    {
        row.resize( mDimX );
    }
}

bool GeneratorData::IsFilled( int32_t x, int32_t y ) const
{
    return GetGCell( x, y ).mFilled;
}

bool GeneratorData::IsRoomIdentical( int32_t x, int32_t y, int32_t roomIndex ) const
{
    return GetGCell( x, y ).mGRoomDescIndex == roomIndex;
}

void GeneratorData::PlaceRoom( RoomDesc const& roomDesc, glm::vec2 pos )
{
    for (int32_t ry = 0; ry < roomDesc.GetCellCount(); ++ry)
    {
        for (int32_t rx = 0; rx < roomDesc.GetCellCount(); ++rx)
        {
            if (IsInBounds( glm::vec2( rx + pos.x, ry + pos.y ) )
                && roomDesc.IsFilled( rx, ry )
                && !IsFilled( rx + pos.x, ry + pos.y ))
            {
                auto& cell = GetGCell( rx + pos.x, ry + pos.y );
                cell.mFilled = true;
                cell.mGRoomDescIndex = mGRoomDescs.size();
                L1( "%d,%d is now filled %d\n", rx + pos.x, ry + pos.y, cell.mFilled );
            }
        }
    }
    GRoomDesc gRoomDesc;
    gRoomDesc.mRoomCoord = pos;
    gRoomDesc.mRoomDesc = roomDesc;
    gRoomDesc.mRoomDesc.GetProperties().clear();
    gRoomDesc.mRoomDesc.ClearAllCellEntrances();
    mGRoomDescs.push_back( gRoomDesc );
    GenerateGraph();
}

bool GeneratorData::CanPlaceRoom( RoomDesc const& roomDesc, glm::vec2 pos ) const
{
    for (int32_t ry = 0; ry < roomDesc.GetCellCount(); ++ry)
    {
        for (int32_t rx = 0; rx < roomDesc.GetCellCount(); ++rx)
        {
            if (roomDesc.IsFilled( rx, ry ))
            {
                if (!IsInBounds( glm::vec2( rx + pos.x, ry + pos.y ) ))
                {
                    L1( "%d,%d is out of bounds\n", rx + pos.x, ry + pos.y );
                    return false; //cell is out of bounds
                }
                if (IsFilled( rx + pos.x, ry + pos.y ))
                {
                    L1( "%d,%d is already filled %d\n", rx + pos.x, ry + pos.y, IsFilled( rx + pos.x, ry + pos.y ) );
                    return false; //cell is already filled this room cant be placed
                }
            }
        }
    }
    return true;
}

bool GeneratorData::IsInBounds( glm::vec2 pos ) const
{
    return pos.x >= 0 && pos.y >= 0 && pos.x < mDimX && pos.y < mDimY;
}

void GeneratorData::ClearRoomProperties( int32_t roomIndex )
{
    mGRoomDescs[roomIndex].mRoomDesc.ClearProperties();
}

void GeneratorData::AddRoomProperty( int32_t roomIndex, RoomDesc::Property prop )
{
    mGRoomDescs[roomIndex].mRoomDesc.AddProperty( prop );
}

int32_t GeneratorData::GetRoomCount() const
{
    return mGRoomDescs.size();
}

Opt<IRoom> GeneratorData::GetRoom( int32_t roomIndex )
{
    return mGRoomDescs[roomIndex].mRoomDesc.GetRoom();
}

RoomDesc const& GeneratorData::GetRoomDesc( int32_t roomIndex ) const
{
    return mGRoomDescs[roomIndex].mRoomDesc;
}

map::RoomDesc& GeneratorData::GetRoomDesc( int32_t roomIndex )
{
    return mGRoomDescs[roomIndex].mRoomDesc;
}

glm::vec2 GeneratorData::GetRoomCoord( int32_t roomIndex ) const
{
    return mGRoomDescs[roomIndex].mRoomCoord;
}

void GeneratorData::LinkCells( glm::vec2 posA, glm::vec2 posB )
{
    if (posA.x > posB.x)
    {
        InsertEntrance( posA, Cell::Left );
        InsertEntrance( posB, Cell::Right );
    }
    else if (posA.y > posB.y)
    {
        InsertEntrance( posA, Cell::Bottom );
        InsertEntrance( posB, Cell::Top );
    }
    else if (posA.x < posB.x)
    {
        InsertEntrance( posA, Cell::Right );
        InsertEntrance( posB, Cell::Left );
    }
    else
    {
        InsertEntrance( posA, Cell::Top );
        InsertEntrance( posB, Cell::Bottom );
    }
}

void GeneratorData::ShuffleNeighbours()
{
    mGraph.ShuffleNodeNeighbours();
}

int32_t GeneratorData::GetNeighbourCount( int32_t roomIndex ) const
{
    return mGraph[roomIndex].Size();
}

int32_t GeneratorData::GetNeigbourRoomIndex( int32_t roomIndex, int32_t neighbourIndex ) const
{
    return mGraph[roomIndex][neighbourIndex];
}

void GeneratorData::AddCellPair( CellPairs_t& cellPairs, glm::vec2 posA, glm::vec2 posB, int32_t room )
{
    if (IsInBounds( posB )
        && IsRoomIdentical( posB.x, posB.y, room ))
    {
        cellPairs.push_back( CellPair_t( posA, posB ) );
    }
}

NeighbourRooms_t GeneratorData::GetNeighbourRooms( int32_t roomIndex )
{
    NeighbourRooms_t r;
    auto& roomDesc = mGRoomDescs[roomIndex];
    for (auto& n : GetRoom( roomIndex )->GetNeighbours())
    {
        glm::vec2 pos = GetRoomCoord(roomIndex) + n;
        if (IsInBounds( pos ))
        {
            int32_t roomIndex = GetGCell( pos.x, pos.y ).mGRoomDescIndex;
            if (std::find( r.begin(), r.end(), roomIndex ) == r.end())
            {
                r.push_back( roomIndex );
            }
        }
    }
    return r;
}

void GeneratorData::GenerateGraph()
{
    mGraph.Clear();
    for (int i = 0; i < mGRoomDescs.size(); ++i)
    {
        auto& neighbourRooms = GetNeighbourRooms( i );
        mGraph.AddNode( GGraphNode( i, GetNeighbourRooms( i ) ) );
    }
}

GRoomDesc& GeneratorData::GetGRoomDesc( glm::vec2 pos )
{
    return mGRoomDescs[GetGCell( pos.x, pos.y ).mGRoomDescIndex];
}

Cell& GeneratorData::GetCell( glm::vec2 pos )
{
    return GetGRoomDesc( pos ).mRoomDesc.GetCell( GetCellCoord( pos ).x, GetCellCoord( pos ).y );
}

glm::vec2 GeneratorData::GetCellCoord( glm::vec2 pos )
{
    return pos-GetGRoomDesc( pos ).mRoomCoord;
}


RoomDesc& GeneratorData::GetRoomDesc( glm::vec2 pos )
{
    return GetGRoomDesc( pos ).mRoomDesc;
}

void GeneratorData::InsertEntrance( glm::vec2 pos, Cell::Entrance entrance )
{
    GetRoomDesc( pos ).AddCellEntrance( GetCellCoord( pos ), entrance );
}


GeneratorData::CellPairs_t GeneratorData::GetCellPairs( int32_t roomA, int32_t roomB )
{
    CellPairs_t r;
    auto const& roomDescA = GetRoomDesc( roomA );
    int32_t cellCount = roomDescA.GetCellCount();
    for (int32_t ry = 0; ry < cellCount; ++ry)
    {
        for (int32_t rx = 0; rx < cellCount; ++rx)
        {
            if (roomDescA.IsFilled( rx, ry ))
            {
                auto roomCoordA = GetRoomCoord( roomA );
                auto vec = roomCoordA + glm::vec2( rx, ry );
                AddCellPair( r, vec, glm::vec2( vec.x - 1, vec.y ), roomB );
                AddCellPair( r, vec, glm::vec2( vec.x + 1, vec.y ), roomB );
                AddCellPair( r, vec, glm::vec2( vec.x, vec.y - 1 ), roomB );
                AddCellPair( r, vec, glm::vec2( vec.x, vec.y + 1 ), roomB );
            }
        }
    }
    return r;
}

}
