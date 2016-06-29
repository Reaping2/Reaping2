#include "i_level_generator.h"
#include "platform/auto_id.h"
#include "random.h"

using platform::AutoId;

namespace map {

ILevelGenerator::ILevelGenerator( int32_t Id )
    : mId( Id )
    , mScene( Scene::Get() )
{
}


ILevelGenerator::~ILevelGenerator()
{

}


bool ILevelGenerator::IsInBounds( glm::vec2 pos ) const
{
    return pos.x >= 0 && pos.y >= 0 && pos.x < mCellCount && pos.y < mCellCount;
}

void ILevelGenerator::AddPossibleRoom( int32_t roomId, int32_t possibility )
{
    for (int i = 0; i < possibility; ++i)
    {
        mPossibleRooms.push_back( roomId );
    }
}


map::GeneratorCell& ILevelGenerator::GetCell( int32_t x, int32_t y )
{
    return mCells[y][x];
}


map::GeneratorCell const& ILevelGenerator::GetCell( int32_t x, int32_t y ) const
{
    return mCells[y][x];
}


bool ILevelGenerator::CanPlaceRoom( RoomDesc const& roomDesc, glm::vec2 pos )
{
    for (int32_t ry = 0; ry < roomDesc.GetCellCount(); ++ry)
    {
        for (int32_t rx = 0; rx < roomDesc.GetCellCount(); ++rx)
        {
            if (roomDesc.GetCell( rx, ry ).mFilled)
            {
                if (!IsInBounds( glm::vec2(rx + pos.x, ry + pos.y) ) )
                {
                    L1( "%d,%d is out of bounds\n", rx + pos.x, ry + pos.y );
                    return false; //cell is out of bounds
                }
                if (GetCell( rx + pos.x, ry + pos.y ).mFilled)
                {
                    L1( "%d,%d is already filled %d\n", rx + pos.x, ry + pos.y, GetCell( rx + pos.x, ry + pos.y ).mFilled );
                    return false; //cell is already filled this room cant be placed
                }
            }
        }
    }
    return true;
}


void ILevelGenerator::PlaceRoom( RoomDesc const& roomDesc, glm::vec2 pos )
{
    for (int32_t ry = 0; ry < roomDesc.GetCellCount(); ++ry)
    {
        for (int32_t rx = 0; rx < roomDesc.GetCellCount(); ++rx)
        {
            if (IsInBounds( glm::vec2( rx + pos.x, ry + pos.y ) )
                && roomDesc.GetCell( rx, ry ).mFilled
                && !GetCell( rx + pos.x, ry + pos.y ).mFilled)
            {
                auto& cell = GetCell( rx + pos.x, ry + pos.y );
                cell.mDescCoord = glm::vec2( rx, ry );
                cell.mFilled = true;
                cell.mGeneratorRoomDescIndex = mRoomDescs.size();
                L1( "%d,%d is now filled %d\n", rx + pos.x, ry + pos.y, cell.mFilled );
            }
        }
    }
    GeneratorRoomDesc gRoomDesc;
    gRoomDesc.mRoomCoord = pos;
    gRoomDesc.mRoomDesc = roomDesc;
    gRoomDesc.mRoomDesc.GetProperties().clear();
    gRoomDesc.mRoomDesc.ClearAllCellEntrances();
    mRoomDescs.push_back( gRoomDesc );
}


ILevelGenerator::CellPairs_t ILevelGenerator::GetCellPairs( int32_t roomA, int32_t roomB )
{
    CellPairs_t r;
    auto& roomDescA=mRoomDescs[roomA];

    for (int32_t ry = 0; ry < roomDescA.mRoomDesc.GetCellCount(); ++ry)
    {
        for (int32_t rx = 0; rx < roomDescA.mRoomDesc.GetCellCount(); ++rx)
        {
            auto& vec = glm::vec2( rx + roomDescA.mRoomCoord.x, ry + roomDescA.mRoomCoord.y );
            auto& cell = roomDescA.mRoomDesc.GetCell( rx, ry );
            if (cell.mFilled)
            {
                AddCellPair( r, vec.x - 1, vec.y, roomA, roomB );
                AddCellPair( r, vec.x + 1, vec.y, roomA, roomB );
                AddCellPair( r, vec.x, vec.y - 1, roomA, roomB );
                AddCellPair( r, vec.x, vec.y + 1, roomA, roomB );
            }
        }
    }
    return r;
}

void ILevelGenerator::AddCellPair( CellPairs_t& cellPairs, int32_t x, int32_t y, int32_t roomA, int32_t roomB )
{
    auto& roomDescA = mRoomDescs[roomA];
    auto& roomDescB = mRoomDescs[roomB];
    if (IsInBounds( glm::vec2(x,y) ) 
        && GetCell( x, y ).mGeneratorRoomDescIndex == roomB)
    {
        cellPairs.push_back( 
            CellPair_t(
                glm::vec2( roomDescA.mRoomCoord.x, roomDescA.mRoomCoord.y ),
                glm::vec2( roomDescB.mRoomCoord.x, roomDescB.mRoomCoord.y )));
    }
}


GGraphNode::GGraphNode( int32_t ind, NeighbourRooms_t const& nodes )
    : mIndex(ind)
    , mNeighbours(nodes)
{

}


void GGraphNode::ShuffleNeighbours()
{
    std::shuffle( mNeighbours.begin(), mNeighbours.end(), mRand );
}

void GGraph::ShuffleNodeNeighbours()
{
    for (auto& node : mNodes)
    {
        node.ShuffleNeighbours();
    }
}

} // namespace map

