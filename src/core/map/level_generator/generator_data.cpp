#include "generator_data.h"
#include "random.h"
#include "room_repo.h"

namespace map {

GeneratorData::GeneratorData()
{

}

GCell& GeneratorData::GetGCell( glm::vec2 pos )
{
    return mGCells[pos.y][pos.x];
}

GCell const& GeneratorData::GetGCell( glm::vec2 pos ) const
{
    return mGCells[pos.y][pos.x];
}

void GeneratorData::SetDimensions( int32_t x, int32_t y )
{
    mDimX = x;
    mDimY = y;
    mGCells.clear();
    mGCells.resize( mDimY, std::vector<GCell>(mDimX) );
}

bool GeneratorData::IsFilled( glm::vec2 pos ) const
{
    return GetGCell( pos ).mFilled;
}

bool GeneratorData::IsRoomIdentical( glm::vec2 pos, int32_t roomIndex ) const
{
    return GetGCell( pos ).mGRoomDescIndex == roomIndex;
}

void GeneratorData::PlaceRoom( RoomDesc const& roomDesc, glm::vec2 pos, PossibleRooms const& possibleRooms )
{
    for (int32_t ry = 0; ry < roomDesc.GetCellCount(); ++ry)
    {
        for (int32_t rx = 0; rx < roomDesc.GetCellCount(); ++rx)
        {
            glm::vec2 relPos = glm::vec2( rx, ry );
            glm::vec2 targetPos = pos + relPos;
            if (IsInBounds( targetPos )
                && roomDesc.IsFilled( relPos )
                && !IsFilled( targetPos ))
            {
                auto& cell = GetGCell( targetPos );
                cell.mFilled = true;
                cell.mGRoomDescIndex = mGRoomDescs.size();
                L2( "%d,%d is now filled %d\n", targetPos.x, targetPos.y, cell.mFilled );
            }
        }
    }
    GRoomDesc gRoomDesc;
    gRoomDesc.mRoomCoord = pos;
    gRoomDesc.mRoomDesc = roomDesc;
    gRoomDesc.mRoomDesc.ClearProperties();
    gRoomDesc.mRoomDesc.ClearCellEntrances();
    gRoomDesc.mPossibleRooms = possibleRooms;
    gRoomDesc.mIsReplaceable = possibleRooms.IsReplaceable( gRoomDesc.mRoomDesc.GetRoom()->GetId() );
    mGRoomDescs.push_back( gRoomDesc );
    GenerateGraph();
}


void GeneratorData::ReplaceRoom( int32_t roomIndex, int32_t roomId )
{
    //auto& gRoomDesc = mGRoomDescs[roomIndex];
    static auto& mRoomRepo = RoomRepo::Get();
    GetRoomDesc(roomIndex).SetRoom(&mRoomRepo( roomId ) );
}

bool GeneratorData::CanPlaceRoom( RoomDesc const& roomDesc, glm::vec2 pos, PossibleRooms const& possibleRooms ) const
{
    for (int32_t ry = 0; ry < roomDesc.GetCellCount(); ++ry)
    {
        for (int32_t rx = 0; rx < roomDesc.GetCellCount(); ++rx)
        {
            glm::vec2 relPos = glm::vec2( rx, ry );
            glm::vec2 targetPos = pos + relPos;
            if (roomDesc.IsFilled( relPos ))
            {
                if (!IsInBounds( targetPos ))
                {
                    L2( "%d,%d is out of bounds\n", targetPos.x, targetPos.y );
                    return false; //cell is out of bounds
                }
                if (IsFilled( targetPos ))
                {
                    L2( "%d,%d is already filled %d\n", targetPos.x, targetPos.x, IsFilled( targetPos ) );
                    return false; //cell is already filled this room can't be placed
                }
            }
        }
    }
    return possibleRooms.IsReplaceable( roomDesc.GetRoom()->GetId() )
            || !HasUnreplaceableNeighbor(*roomDesc.GetRoom(),pos);
}

bool GeneratorData::IsInBounds( glm::vec2 pos ) const
{
    return pos.x >= 0 && pos.y >= 0 && pos.x < mDimX && pos.y < mDimY;
}

void GeneratorData::ClearRoomProperties( int32_t roomIndex )
{
    mGRoomDescs[roomIndex].mRoomDesc.ClearProperties();
}

void GeneratorData::AddRoomProperty( int32_t roomIndex, RoomProperty::Type prop )
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

RoomDesc& GeneratorData::GetRoomDesc( int32_t roomIndex )
{
    return mGRoomDescs[roomIndex].mRoomDesc;
}


RoomDesc const& GeneratorData::GetRoomDesc( glm::vec2 pos ) const
{
    return GetGRoomDesc( pos ).mRoomDesc;
}

glm::vec2 GeneratorData::GetRoomCoord( int32_t roomIndex ) const
{
    return mGRoomDescs[roomIndex].mRoomCoord;
}

void GeneratorData::LinkCells( glm::vec2 posA, glm::vec2 posB )
{
    auto& cellA = GetCell( posA );
    auto& cellB = GetCell( posB );
    if (posA.x > posB.x)
    {
        cellA.AddEntrance( EntranceType::Left );
        cellB.AddEntrance( EntranceType::Right );
    }
    else if (posA.y > posB.y)
    {
        cellA.AddEntrance( EntranceType::Bottom );
        cellB.AddEntrance( EntranceType::Top );
    }
    else if (posA.x < posB.x)
    {
        cellA.AddEntrance( EntranceType::Right );
        cellB.AddEntrance( EntranceType::Left );
    }
    else
    {
        cellA.AddEntrance( EntranceType::Top );
        cellB.AddEntrance( EntranceType::Bottom );
    }
}

void GeneratorData::ShuffleNeighbours()
{
    mGraph.ShuffleNodeNeighbours();
}

int32_t GeneratorData::GetNeighbourRoomCount( int32_t roomIndex ) const
{
    return mGraph[roomIndex].Size();
}

int32_t GeneratorData::GetNeigbourRoomIndex( int32_t roomIndex, int32_t neighbourIndex ) const
{
    return mGraph[roomIndex][neighbourIndex];
}


NeighbourRooms_t const& GeneratorData::GetNeighbourRooms( int32_t roomIndex ) const
{
    return mGraph[roomIndex].mNeighbours;
}

void GeneratorData::AddCellPair( CellPairs_t& cellPairs, glm::vec2 posA, glm::vec2 posB, int32_t room ) const
{
    if (IsInBounds( posB )
        && IsRoomIdentical( posB, room ))
    {
        cellPairs.push_back( CellPair_t( posA, posB ) );
    }
}

NeighbourRooms_t GeneratorData::GetPossibleNeighbourRooms( int32_t roomIndex )
{
    NeighbourRooms_t r;
    for (auto& n : GetRoom( roomIndex )->GetNeighbourCells())
    {
        glm::vec2 pos = GetRoomCoord(roomIndex) + n;
        if (IsInBounds( pos ))
        {
            int32_t roomIndex = GetGCell( pos ).mGRoomDescIndex;
            if (roomIndex!=-1&&std::find( r.begin(), r.end(), roomIndex ) == r.end())
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
        bool const replI = GetGRoomDesc( i ).mIsReplaceable;
        NeighbourRooms_t neighbourRooms;
        for (auto&& neighbourRoom : GetPossibleNeighbourRooms( i ))
        {
            bool const replNeigh = GetGRoomDesc( neighbourRoom ).mIsReplaceable;
            for (auto&& cellPair : GetAdjacentCellPairs( i, neighbourRoom ))
            {
                if (CanLinkCells( cellPair.first, cellPair.second, replI, replNeigh ))
                {
                    neighbourRooms.push_back( neighbourRoom );
                    break;
                }
            }
        }
        mGraph.AddNode( GGraphNode( i, neighbourRooms ) );
    }
}


void GeneratorData::GenerateRouteGraph()
{
    mGraph.Clear();
    for (int i = 0; i < mGRoomDescs.size(); ++i)
    {
        NeighbourRooms_t neighbourRooms;
        for (auto&& neighbourRoom : GetPossibleNeighbourRooms( i ))
        {
            for (auto&& cellPair : GetAdjacentCellPairs( i, neighbourRoom ))
            {
                if (AreCellsLinked( cellPair.first, cellPair.second ))
                {
                    neighbourRooms.push_back( neighbourRoom );
                    break;
                }
            }
        }
        mGraph.AddNode( GGraphNode( i, neighbourRooms ) );
    }
}

bool GeneratorData::CanLinkCells( glm::vec2 posA, glm::vec2 posB, bool replaceableA /*= false*/, bool replaceableB /*= false*/ ) const
{
    auto const& currCellA = GetCell( posA );
    auto const& currCellB = GetCell( posB );
    // getting the original rooms roomdesc and it's properties for this cell
    auto const& cellA = GetRoomDesc( posA ).GetRoom()->GetRoomDesc().GetCell( currCellA.mDescCoord );
    auto const& cellB = GetRoomDesc( posB ).GetRoom()->GetRoomDesc().GetCell( currCellB.mDescCoord );
    if (posA.x > posB.x)
    {
        return  (replaceableA||cellA.HasEntrance( EntranceType::Left ))
            && (replaceableB||cellB.HasEntrance( EntranceType::Right ));
    }
    else if (posA.y > posB.y)
    {
        return (replaceableA || cellA.HasEntrance( EntranceType::Bottom ))
            && (replaceableB || cellB.HasEntrance( EntranceType::Top ));
    }
    else if (posA.x < posB.x)
    {
        return (replaceableA || cellA.HasEntrance( EntranceType::Right ))
            && (replaceableB || cellB.HasEntrance( EntranceType::Left ));
    }
    return (replaceableA || cellA.HasEntrance( EntranceType::Top ))
        && (replaceableB || cellB.HasEntrance( EntranceType::Bottom ));
}


bool GeneratorData::AreCellsLinked( glm::vec2 posA, glm::vec2 posB ) const
{
    auto const& cellA = GetCell( posA );
    auto const& cellB = GetCell( posB );
    if (posA.x > posB.x)
    {
        return  ( cellA.HasEntrance( EntranceType::Left ))
            && ( cellB.HasEntrance( EntranceType::Right ));
    }
    else if (posA.y > posB.y)
    {
        return ( cellA.HasEntrance( EntranceType::Bottom ))
            && ( cellB.HasEntrance( EntranceType::Top ));
    }
    else if (posA.x < posB.x)
    {
        return ( cellA.HasEntrance( EntranceType::Right ))
            && ( cellB.HasEntrance( EntranceType::Left ));
    }
    return ( cellA.HasEntrance( EntranceType::Top ))
        && ( cellB.HasEntrance( EntranceType::Bottom ));
}

GRoomDesc const& GeneratorData::GetGRoomDesc( glm::vec2 pos ) const
{
    return mGRoomDescs[GetGCell( pos ).mGRoomDescIndex];
}


GRoomDesc const& GeneratorData::GetGRoomDesc( int32_t roomIndex ) const
{
    return mGRoomDescs[roomIndex];
}


map::GRoomDesc& GeneratorData::GetGRoomDesc( glm::vec2 pos )
{
    return mGRoomDescs[GetGCell( pos ).mGRoomDescIndex];
}


bool GeneratorData::HasGRoomDesc( glm::vec2 pos ) const
{
    return GetGCell( pos ).mGRoomDescIndex != -1;
}

bool GeneratorData::HasUnreplaceableNeighbor( IRoom const& room, glm::vec2 pos ) const
{
    return std::find_if(room.GetNeighbourCells().begin(), room.GetNeighbourCells().end(), [&]( glm::vec2 n ) 
            {
                auto const v = pos + n;
                return IsInBounds( v ) && HasGRoomDesc( v ) && !GetGRoomDesc( v ).mIsReplaceable;
            }) != room.GetNeighbourCells().end();
}


GGraph const& GeneratorData::GetGraph() const
{
    return mGraph;
}

Cell& GeneratorData::GetCell( glm::vec2 pos )
{
    return GetRoomDesc( pos ).GetCell( GetCellCoord( pos ) );
}


Cell const& GeneratorData::GetCell( glm::vec2 pos ) const
{
    return GetRoomDesc( pos ).GetCell( GetCellCoord( pos ) );
}

glm::vec2 GeneratorData::GetCellCoord( glm::vec2 pos ) const
{
    return pos-GetGRoomDesc( pos ).mRoomCoord;
}


RoomDesc& GeneratorData::GetRoomDesc( glm::vec2 pos )
{
    return GetGRoomDesc( pos ).mRoomDesc;
}

GeneratorData::CellPairs_t GeneratorData::GetAdjacentCellPairs( int32_t roomA, int32_t roomB )
{
    CellPairs_t r;
    auto const& roomDescA = GetRoomDesc( roomA );
    auto roomCoordA = GetRoomCoord( roomA );
    int32_t cellCount = roomDescA.GetCellCount();
    for (int32_t ry = 0; ry < cellCount; ++ry)
    {
        for (int32_t rx = 0; rx < cellCount; ++rx)
        {
            if (roomDescA.IsFilled( rx, ry ))
            {
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


GGraphNode::GGraphNode( int32_t ind, NeighbourRooms_t const& nodes )
    : mIndex( ind )
    , mNeighbours( nodes )
{

}


int32_t GGraphNode::operator[]( int32_t neighbourIndex ) const
{
    return mNeighbours[neighbourIndex];
}


int32_t GGraphNode::Size() const
{
    return mNeighbours.size();
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


GGraphNode const& GGraph::operator[]( int32_t nodeIndex ) const
{
    return mNodes[nodeIndex];
}


void GGraph::Clear()
{
    mNodes.clear();
}


void GGraph::AddNode( GGraphNode const& node )
{
    mNodes.push_back( node );
}

}
