#include "jungle_level_generator.h"
#include "platform/auto_id.h"
#include "../../i_position_component.h"
#include "random.h"
#include "level_generated_event.h"
#include "platform/settings.h"
#include <chrono>
#include <stack>

using platform::AutoId;

namespace map {

JungleLevelGenerator::JungleLevelGenerator( int32_t Id )
    : ILevelGenerator( Id )
    , mActorFactory( ActorFactory::Get() )
    , mRoomRepo( RoomRepo::Get() )
{
    mCellSize = 1000;
    mCellCount = 5;
    mRand.seed( Settings::Get().GetUInt("generator.seed", unsigned(std::time(0))) );
    AddPossibleRoom( AutoId( "simple_room1" ), 3);
    AddPossibleRoom( AutoId( "vdouble_room1" ), 1);
    AddPossibleRoom( AutoId( "hdouble_room1" ), 1 );
}

void JungleLevelGenerator::Generate()
{
    mCells.resize( mCellCount );
    for (auto& row : mCells)
    {
        row.resize( mCellCount );
    }
    mFreeNodes.push_back( glm::vec2(0,0) );
    PlaceRooms();
    CreateStartAndEnd();
    GenerateGraph();
    CreateRoute();
    LinkRooms();
    GenerateTerrain();
    EventServer<LevelGeneratedEvent>::Get().SendEvent( LevelGeneratedEvent() );
}


void JungleLevelGenerator::GenerateTerrain()
{
    for (auto& roomDesc : mRoomDescs)
    {
        roomDesc.mRoomDesc.GetRoom()->Generate(
            roomDesc.mRoomDesc
            , roomDesc.mRoomCoord.x*mCellSize - 2500
            , roomDesc.mRoomCoord.y*mCellSize - 2500 );
    }
}

void JungleLevelGenerator::PlaceRooms()
{
    while (!mFreeNodes.empty())
    {
        glm::vec2 vec = mFreeNodes.front();
        if (GetCell( vec.x, vec.y ).mFilled)
        {
            LogNodes( "filled so pre_pop" ); mFreeNodes.pop_front(); LogNodes( "filled so post_pop" );
            continue;
        }
        Opt<IRoom> placedRoom = PlaceARoom( vec );
        if (placedRoom.IsValid())
        {
            LogNodes( "pre_pop" ); mFreeNodes.pop_front(); LogNodes( "post_pop" );
            auto& neighbours = placedRoom->GetNeighbours();
            for (auto& nVec : neighbours)
            {
                auto v=nVec + vec;
                if (IsInBounds( v )
                    &&!GetCell( v.x, v.y ).mFilled
                    &&std::find(mFreeNodes.begin(),mFreeNodes.end(),v)==mFreeNodes.end())
                {
                    mFreeNodes.push_back( v );
                }
            }
        }
        else
        {
            BOOST_ASSERT( false );
        }
    }
}

Opt<IRoom> JungleLevelGenerator::PlaceARoom( glm::vec2 &vec )
{
    Opt<IRoom> r;
    std::shuffle( mPossibleRooms.begin(), mPossibleRooms.end(), mRand );
    bool succ = false;
    for (auto& roomId:mPossibleRooms)
    {
        auto& room = mRoomRepo( roomId );
        if (CanPlaceRoom(room.GetRoomDesc(),vec ))
        {
            PlaceRoom( room.GetRoomDesc(), vec );
            r = &room;
            break;
        }
    }
    return r;
}

void JungleLevelGenerator::LogNodes( std::string log )
{
    L1( "%d %s\n", mFreeNodes.size(), log.c_str() );
    for (auto& fn : mFreeNodes)
    {
        L1( "(%f,%f)", fn.x, fn.y );
    }
    L1( "\n" );
}


void JungleLevelGenerator::GenerateGraph()
{
    mGraph.mNodes.clear();
    for (int i = 0; i < mRoomDescs.size(); ++i)
    {
        auto& neighbourRooms = GetNeighbourRooms( i );
        mGraph.mNodes.push_back( GGraphNode( i, GetNeighbourRooms( i ) ) );
    }
}


NeighbourRooms_t JungleLevelGenerator::GetNeighbourRooms( int32_t roomIndex )
{
    NeighbourRooms_t r;
    auto& roomDesc = mRoomDescs[roomIndex];
    for (auto& n : roomDesc.mRoomDesc.GetRoom()->GetNeighbours())
    {
        glm::vec2 pos = roomDesc.mRoomCoord + n;
        if (IsInBounds(pos))
        {
            int32_t roomIndex = GetCell( pos.x, pos.y ).mGeneratorRoomDescIndex;
            if (std::find( r.begin(), r.end(), roomIndex ) == r.end())
            {
                r.push_back( roomIndex );
            }
        }
    }
    return r;
}


void JungleLevelGenerator::CreateRoute()
{
    mGraph.ShuffleNodeNeighbours();
    Route_t route;
    mRoute.swap( route );
    int32_t curr = mStartIndex;
    mRoute.push( curr );
    std::vector<int32_t> visit(mRoomDescs.size(), 0);
    std::set<int32_t> visited;
    int32_t minLength = 5;
    int32_t endChance = 80;
    int32_t chanceIncrease = 10;
    bool endHit = false;
    while (curr != -1 && !endHit)
    {
        while (visit[curr] < mGraph.mNodes[curr].mNeighbours.size()
            &&visited.find(visit[curr])!=visited.end())
        {
            ++visit[curr];
        }
            
        if (visit[curr] >= mGraph.mNodes[curr].mNeighbours.size())
        {
            visit[curr] = 0;
            visited.erase( curr );
            mRoute.pop();
            curr=mRoute.empty()?-1:mRoute.top();
        }
        else
        {
            int32_t nextRoomIndex = mGraph.mNodes[curr].mNeighbours[visit[curr]];
            visited.insert( nextRoomIndex );
            curr = nextRoomIndex;
            mRoute.push( curr );
            if ((int32_t)mRoute.size() - minLength>0)
            {
                if (mRand() % (100) < endChance + ((int32_t)mRoute.size() - minLength)*chanceIncrease)
                {
                    endHit = true;
                }
            }
        }
    }
    if (endHit)
    {
        mEndIndex = mRoute.top();
        auto& endRoom = mRoomDescs.at( mEndIndex );
        //TODO: check if it has a emd property
        endRoom.mRoomDesc.GetProperties().clear();
        endRoom.mRoomDesc.GetProperties().insert( RoomDesc::End );
    }
    else
    {
        BOOST_ASSERT( false ); // the longest route is too short
    }
}


void JungleLevelGenerator::LinkRooms()
{
    int32_t roomB = mRoute.top();
    mRoute.pop();
    int32_t roomA = -1;
    while (!mRoute.empty())
    {
        roomA = roomB;
        roomB = mRoute.top();
        mRoute.pop();
        auto cellPairs = GetCellPairs( roomA, roomB );
        auto& cellPair = cellPairs[mRand() % cellPairs.size()];
        if (cellPair.first.x > cellPair.second.x)
        {
            InsertEntrance( cellPair.first, Cell::Left );
            InsertEntrance( cellPair.second, Cell::Right );
        }
        else if (cellPair.first.y > cellPair.second.y)
        {
            InsertEntrance( cellPair.first, Cell::Top );
            InsertEntrance( cellPair.second, Cell::Bottom );
        }
        else if (cellPair.first.x < cellPair.second.x)
        {
            InsertEntrance( cellPair.first, Cell::Right );
            InsertEntrance( cellPair.second, Cell::Left );
        }
        else
        {
            InsertEntrance( cellPair.first, Cell::Bottom );
            InsertEntrance( cellPair.second, Cell::Top );
        }
    }
}

void JungleLevelGenerator::InsertEntrance( glm::vec2 pos, Cell::Entrance entrance )
{
    auto& gCell=GetCell( pos.x, pos.y );
    mRoomDescs[gCell.mGeneratorRoomDescIndex]
        .mRoomDesc.GetCell( gCell.mDescCoord.x, gCell.mDescCoord.y )
        .mPossibleEntrances.insert( entrance );
}

void JungleLevelGenerator::CreateStartAndEnd()
{
    mStartIndex = mRand() % mRoomDescs.size();
    while (mStartIndex == mEndIndex)
    {
        mEndIndex = mRand() % mRoomDescs.size();
    }
    auto& startRoom = mRoomDescs.at( mStartIndex );
    //TODO: check if it has a start property
    startRoom.mRoomDesc.GetProperties().clear();
    startRoom.mRoomDesc.GetProperties().insert( RoomDesc::Start );
}

} // namespace map

