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

    {
        RouteProperties properties;
        properties.minLength = 13;
        properties.endChance = 30;
        properties.chanceIncrease = 30;
        auto route = CreateRoute( mStartIndex, properties );
        LinkRooms( route );
        mEndIndex = route.top();
        auto& endRoom = mRoomDescs.at( mEndIndex );
        //TODO: check if it has a end property
        endRoom.mRoomDesc.GetProperties().clear();
        endRoom.mRoomDesc.GetProperties().insert( RoomDesc::End );
    }
    int32_t index = 0;
    std::shuffle( mVisited.begin(), mVisited.end(), mRand );
    RouteProperties properties;
    properties.minLength = 2;
    properties.endChance = 60;
    properties.chanceIncrease = 0;
    while (index < mVisited.size())
    {
        auto route = CreateRoute( mVisited[index], properties );
        if (route.size()==1)
        {
            ++index;
        }
        else
        {
            LinkRooms( route );
            index = 0;
            std::shuffle( mVisited.begin(), mVisited.end(), mRand );
        }
       
    }

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
    mGraph.Clear();
    for (int i = 0; i < mRoomDescs.size(); ++i)
    {
        auto& neighbourRooms = GetNeighbourRooms( i );
        mGraph.AddNode( GGraphNode( i, GetNeighbourRooms( i ) ) );
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


JungleLevelGenerator::Route_t JungleLevelGenerator::CreateRoute( int32_t startIndex, RouteProperties const& properties )
{
    mGraph.ShuffleNodeNeighbours();
    Route_t route;
    int32_t curr = startIndex;
    route.push( curr );
    Route_t longestRoute;
    Visited_t longestVisited;
    if (std::find( mVisited.begin(), mVisited.end(), curr ) == mVisited.end())
    {
        mVisited.push_back( curr );
    }
    std::vector<int32_t> nextNeigh(mRoomDescs.size(), 0);
    bool endHit = false;
    while (curr != -1 && !endHit)
    {
        while (nextNeigh[curr] < mGraph[curr].Size()
            &&std::find(mVisited.begin(),mVisited.end(),
                mGraph[curr][nextNeigh[curr]]) != mVisited.end()
)
        {
            ++nextNeigh[curr];
        }
            
        if(nextNeigh[curr] >= mGraph[curr].Size())
        {
            if (properties.minLength == 0)
            {
                endHit = true;
            }
            else
            {
                nextNeigh[curr] = 0;
                if (longestRoute.size() < route.size())
                {
                    longestRoute = route;
                    longestVisited = mVisited;
                }
                mVisited.erase( std::find( mVisited.begin(), mVisited.end(), curr ) );
                route.pop();
                if (route.empty())
                {
                    curr = -1;
                }
                else
                {
                    curr = route.top();
                    ++nextNeigh[curr];
                }
            }
        }
        else
        {
            int32_t nextRoomIndex = mGraph[curr][nextNeigh[curr]];
            mVisited.push_back( nextRoomIndex );
            curr = nextRoomIndex;
            route.push( curr );
            if ((int32_t)route.size() - properties.minLength>0)
            {
                if (mRand() % (100) < properties.endChance + ((int32_t)route.size() - properties.minLength)*properties.chanceIncrease)
                {
                    endHit = true;
                }
            }
        }
    }
    if (route.empty())
    {
        std::swap( mVisited,longestVisited );
        std::swap( longestRoute, route );
    }
    return route;
}


void JungleLevelGenerator::LinkRooms( Route_t route )
{
    int32_t roomB = route.top();
    route.pop();
    int32_t roomA = -1;
    while (!route.empty())
    {
        roomA = roomB;
        roomB = route.top();
        route.pop();
        auto cellPairs = GetCellPairs( roomA, roomB );
        auto& cellPair = cellPairs[mRand() % cellPairs.size()];
        if (cellPair.first.x > cellPair.second.x)
        {
            InsertEntrance( cellPair.first, Cell::Left );
            InsertEntrance( cellPair.second, Cell::Right );
        }
        else if (cellPair.first.y > cellPair.second.y)
        {
            InsertEntrance( cellPair.first, Cell::Bottom );
            InsertEntrance( cellPair.second, Cell::Top );
        }
        else if (cellPair.first.x < cellPair.second.x)
        {
            InsertEntrance( cellPair.first, Cell::Right );
            InsertEntrance( cellPair.second, Cell::Left );
        }
        else
        {
            InsertEntrance( cellPair.first, Cell::Top );
            InsertEntrance( cellPair.second, Cell::Bottom );
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

