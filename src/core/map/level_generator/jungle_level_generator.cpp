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
    AddPossibleRoom( AutoId( "simple_room1" ), 3);
    AddPossibleRoom( AutoId( "vdouble_room1" ), 1);
    AddPossibleRoom( AutoId( "hdouble_room1" ), 1 );
}

void JungleLevelGenerator::Generate()
{
    mGCells.resize( mCellCount );
    for (auto& row : mGCells)
    {
        row.resize( mCellCount );
    }
    PlaceRooms( glm::vec2( 0, 0 ) );
    CreateStart();
    GenerateGraph();

    CreateMainRoute();
    CreateSideRoutes();

    GenerateTerrain();
    EventServer<LevelGeneratedEvent>::Get().SendEvent( LevelGeneratedEvent() );
}


void JungleLevelGenerator::CreateSideRoutes()
{
    int32_t index = 0;
    std::shuffle( mVisited.begin(), mVisited.end(), mRand );
    RouteProperties properties;
    properties.minLength = Settings::Get().GetInt( "generator.side_route.min_length", 2 );;
    properties.endChance = Settings::Get().GetInt( "generator.side_route.end_chance", 30 );;
    properties.chanceIncrease = Settings::Get().GetInt( "generator.side_route.chance_increase", 0 );;
    while (index < mVisited.size())
    {
        auto route = CreateRoute( mVisited[index], properties );
        if (route.size() == 1)
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
}

void JungleLevelGenerator::CreateMainRoute()
{
    RouteProperties properties;
    properties.minLength = Settings::Get().GetInt( "generator.route.min_length", 13 );
    properties.endChance = Settings::Get().GetInt( "generator.route.end_chance", 30 );
    properties.chanceIncrease = Settings::Get().GetInt( "generator.route.chance_increase", 30 );
    auto route = CreateRoute( mStartIndex, properties );
    LinkRooms( route );
    mEndIndex = route.top();
    auto& endRoom = mGRoomDescs.at( mEndIndex );
    //TODO: check if it has a end property
    endRoom.mRoomDesc.GetProperties().clear();
    endRoom.mRoomDesc.GetProperties().insert( RoomDesc::End );
}

void JungleLevelGenerator::GenerateTerrain()
{
    for (auto& roomDesc : mGRoomDescs)
    {
        roomDesc.mRoomDesc.GetRoom()->Generate(
            roomDesc.mRoomDesc
            , roomDesc.mRoomCoord.x*mCellSize - 2400
            , roomDesc.mRoomCoord.y*mCellSize - 2400 );
    }
}

void JungleLevelGenerator::PlaceRooms( glm::vec2 const& startPos )
{
    FreeNodes_t freeNodes;
    freeNodes.push_back( startPos );
    while (!freeNodes.empty())
    {
        glm::vec2 vec = freeNodes.front();
        if (GetCell( vec.x, vec.y ).mFilled)
        {
            LogNodes( "filled so pre_pop", freeNodes ); freeNodes.pop_front(); LogNodes( "filled so post_pop", freeNodes );
            continue;
        }
        Opt<IRoom> placedRoom = PlaceARoom( vec );
        if (placedRoom.IsValid())
        {
            LogNodes( "pre_pop", freeNodes ); freeNodes.pop_front(); LogNodes( "post_pop", freeNodes );
            auto& neighbours = placedRoom->GetNeighbours();
            for (auto& nVec : neighbours)
            {
                auto v=nVec + vec;
                if (IsInBounds( v )
                    &&!GetCell( v.x, v.y ).mFilled
                    &&std::find(freeNodes.begin(),freeNodes.end(),v)==freeNodes.end())
                {
                    freeNodes.push_back( v );
                }
            }
        }
        else
        {
            BOOST_ASSERT( false );
        }
    }
}

Opt<IRoom> JungleLevelGenerator::PlaceARoom( glm::vec2 const& vec )
{
    Opt<IRoom> r;
    std::shuffle( mPossibleRooms.begin(), mPossibleRooms.end(), mRand );
    bool succ = false;
    for (auto& roomId:mPossibleRooms)
    {
        auto& room = mRoomRepo( roomId );
        if (CanPlaceRoom( room.GetRoomDesc(),vec ))
        {
            PlaceRoom( room.GetRoomDesc(), vec );
            r = &room;
            break;
        }
    }
    return r;
}

void JungleLevelGenerator::LogNodes( std::string log, FreeNodes_t const& nodes )
{
    L2( "%d %s\n", nodes.size(), log.c_str() );
    for (auto& fn : nodes)
    {
        L2( "(%f,%f)", fn.x, fn.y );
    }
    L2( "\n" );
}


void JungleLevelGenerator::GenerateGraph()
{
    mGraph.Clear();
    for (int i = 0; i < mGRoomDescs.size(); ++i)
    {
        auto& neighbourRooms = GetNeighbourRooms( i );
        mGraph.AddNode( GGraphNode( i, GetNeighbourRooms( i ) ) );
    }
}


NeighbourRooms_t JungleLevelGenerator::GetNeighbourRooms( int32_t roomIndex )
{
    NeighbourRooms_t r;
    auto& roomDesc = mGRoomDescs[roomIndex];
    for (auto& n : roomDesc.mRoomDesc.GetRoom()->GetNeighbours())
    {
        glm::vec2 pos = roomDesc.mRoomCoord + n;
        if (IsInBounds(pos))
        {
            int32_t roomIndex = GetCell( pos.x, pos.y ).mGRoomDescIndex;
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
    std::vector<int32_t> nextNeigh(mGRoomDescs.size(), 0);
    bool endHit = false;
    while (curr != -1 && !endHit)
    {
        while (nextNeigh[curr] < mGraph[curr].Size()
            &&std::find(mVisited.begin(),mVisited.end(),
                mGraph[curr][nextNeigh[curr]]) != mVisited.end())
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
    mGRoomDescs[gCell.mGRoomDescIndex]
        .mRoomDesc.GetCell( gCell.mDescCoord.x, gCell.mDescCoord.y )
        .mPossibleEntrances.insert( entrance );
}

void JungleLevelGenerator::CreateStart()
{
    mStartIndex = mRand() % mGRoomDescs.size();
    while (mStartIndex == mEndIndex)
    {
        mEndIndex = mRand() % mGRoomDescs.size();
    }
    auto& startRoom = mGRoomDescs.at( mStartIndex );
    //TODO: check if it has a start property
    startRoom.mRoomDesc.GetProperties().clear();
    startRoom.mRoomDesc.GetProperties().insert( RoomDesc::Start );
}

} // namespace map

