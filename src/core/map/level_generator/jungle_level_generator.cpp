#include "jungle_level_generator.h"
#include "platform/auto_id.h"
#include "../../i_position_component.h"
#include "random.h"
#include "level_generated_event.h"
#include "platform/settings.h"
#include <chrono>
#include <stack>
#include <numeric>

using platform::AutoId;

namespace map {

JungleLevelGenerator::JungleLevelGenerator( int32_t Id )
    : ILevelGenerator( Id )
    , mActorFactory( ActorFactory::Get() )
    , mRoomRepo( RoomRepo::Get() )
{
}

void JungleLevelGenerator::Generate()
{
    auto seed = mSeed == 0 ? unsigned( std::time( 0 ) ) : mSeed;
    L2( "Jungle generator with seed: %d", seed );
    mRand.seed( seed );
    int32_t a = mRand() % 10;
    mGData.SetDimensions( mCellCount, mCellCount );
    mFreeCellPositions.clear();

    mStartRoomIndex = PlaceRoomByProperty( RoomProperty::Start );
    PlaceMandatoryRooms();
    PlaceRooms();

    CreateMainRoute();
    CreateSideRoutes();

    CheckRoomEntrances();

    GenerateTerrain();
    EventServer<LevelGeneratedEvent>::Get().SendEvent( LevelGeneratedEvent( LevelGeneratedEvent::TerrainGenerated ) );
}


void JungleLevelGenerator::Load( Json::Value& setters )
{
    ILevelGenerator::Load( setters );
    mMainRouteProperties.Load( setters["route"] );
    mSideRouteProperties.Load( setters["side_route"] );
    mPossibleRooms.Load( setters["possible_rooms"] );
    mMandatoryRooms.clear();
    auto& mandatoryRoomsJson = setters["mandatory_rooms"];
    if (mandatoryRoomsJson.isArray())
    {
        for (auto& possibleRoomsJson : mandatoryRoomsJson)
        {
            PossibleRooms possibleRooms;
            possibleRooms.Load( possibleRoomsJson );
            mMandatoryRooms.push_back( possibleRooms );
        }
    }
}

void JungleLevelGenerator::CreateSideRoutes()
{
    int32_t roomIndex = 0;
    std::shuffle( mVisitedRooms.begin(), mVisitedRooms.end(), mRand );
    while (roomIndex < mVisitedRooms.size())
    {
        auto route = CreateRoute( mVisitedRooms[roomIndex], mSideRouteProperties );
        if (route.size() == 1)
        {
            ++roomIndex;
        }
        else
        {
            LinkRooms( route );
            roomIndex = 0;
            std::shuffle( mVisitedRooms.begin(), mVisitedRooms.end(), mRand );
        }

    }
}

void JungleLevelGenerator::CreateMainRoute()
{
    auto route = CreateRoute( mStartRoomIndex, mMainRouteProperties );
    LinkRooms( route );
    mEndRoomIndex = route.top();
    //TODO: check if it has an end property
    mGData.AddRoomProperty( mEndRoomIndex, RoomProperty::End );
}

void JungleLevelGenerator::GenerateTerrain()
{
    for (int32_t i = 0; i < mGData.GetRoomCount(); ++i)
    {
        mGData.GetRoom(i)->Generate(
            mGData.GetRoomDesc( i )
            , mGData.GetRoomCoord( i )*mCellSize + glm::vec2(mScene.GetDimensions()));
    }
}


void JungleLevelGenerator::RecreateBorders()
{

}



int32_t JungleLevelGenerator::PlaceRoomByProperty( RoomProperty::Type roomProp )
{
    const auto roomIds = mPossibleRooms.GetRoomIdsByProperty( roomProp );
    BOOST_ASSERT( !roomIds.empty() );
    for (auto&& roomId : roomIds)
    {
        const int32_t roomIndex = PlaceRoom( roomId.mRoomId, mPossibleRooms );
        if (roomIndex != -1)
        {
            mGData.AddRoomProperty( roomIndex, roomProp );
            L2( "Succesfully placed room %d\n", roomId );
            return roomIndex;
        }
        else
        {
            L2( "Could not place room %d try next\n", roomId );
        }
    }
    return -1;
}


void JungleLevelGenerator::PlaceMandatoryRooms()
{
    for (auto& possibleRooms : mMandatoryRooms)
    {
        possibleRooms.ShuffleRoomIds();
        for (auto&& roomId : possibleRooms.GetRoomIds())
        {
            if (PlaceRoom( roomId.mRoomId, possibleRooms ) != -1)
            {
                L2( "Succesfully placed room %d\n", roomId );
                break;
            }
            else
            {
                L2( "Could not place room %d try next\n", roomId );
            }
        }
        L2( "End of mandatory room iteration.\n" );
    }
}


int32_t JungleLevelGenerator::PlaceRoom( int32_t roomId, PossibleRooms const& possibleRooms )
{
    auto& room = mRoomRepo( roomId );
    const int32_t maxSize = mCellCount - room.GetRoomDesc().GetCellCount();
    std::vector<int32_t> xvalues( maxSize );
    std::iota( xvalues.begin(), xvalues.end(), 0 );
    std::shuffle( xvalues.begin(), xvalues.end(), mRand );

    std::vector<int32_t> yvalues( maxSize );
    std::iota( yvalues.begin(), yvalues.end(), 0 );
    std::shuffle( yvalues.begin(), yvalues.end(), mRand );

    for (auto x : xvalues)
    {
        for (auto y : yvalues)
        {
            const glm::vec2 pos( x, y );
            if (mGData.CanPlaceRoom( room.GetRoomDesc(), pos, possibleRooms ))
            {
                const int32_t roomIndex = mGData.GetRoomCount();
                mGData.PlaceRoom( room.GetRoomDesc(), pos, possibleRooms );
                AddNeighboursToFreeCells( room, pos );
                return roomIndex;
            }
        }
    }
    return -1;
}


void JungleLevelGenerator::CheckRoomEntrances()
{
    for (int32_t i = 0; i < mGData.GetRoomCount(); ++i)
    {
        auto& roomDesc = mGData.GetRoomDesc( i );
        if (!roomDesc.FitsInto( roomDesc.GetRoom()->GetRoomDesc(), RoomDesc::Layout|RoomDesc::Entrance ))
        {
            L2( "Found a room that should be changed!\n" );
            auto possibleRooms = mGData.GetGRoomDesc(i).mPossibleRooms.GetRoomIdsFiltered( roomDesc, RoomDesc::Layout|RoomDesc::Entrance );
            if (possibleRooms.empty())
            {
                L2( "A room should have been replaced but couldn't find matching room!" );
                continue;
            }
            mGData.ReplaceRoom( i, possibleRooms.at( 0 ).mRoomId );
        }
    }
}

void JungleLevelGenerator::PlaceRooms()
{
    while (!mFreeCellPositions.empty())
    {
        glm::vec2 cellPos = mFreeCellPositions.front();
        if (mGData.IsFilled( cellPos ))
        {
            LogNodes( "filled so pre_pop", mFreeCellPositions ); mFreeCellPositions.pop_front(); LogNodes( "filled so post_pop", mFreeCellPositions );
            continue;
        }
        Opt<IRoom> placedRoom = PlaceRandomRoom( cellPos );
        if (placedRoom.IsValid())
        {
            AddNeighboursToFreeCells( *placedRoom, cellPos );
        }
        else
        {
            BOOST_ASSERT( false );
        }
    }
}


void JungleLevelGenerator::AddNeighboursToFreeCells( IRoom& placedRoom, glm::vec2 const& cellPos )
{
    auto& neighbours = placedRoom.GetNeighbourCells();
    for (auto& neighPos : neighbours)
    {
        auto possiblePos = neighPos + cellPos;
        if (mGData.IsInBounds( possiblePos )
            && !mGData.IsFilled( possiblePos )
            && std::find( mFreeCellPositions.begin(), mFreeCellPositions.end(), possiblePos ) == mFreeCellPositions.end())
        {
            mFreeCellPositions.push_back( possiblePos );
        }
    }
}

Opt<IRoom> JungleLevelGenerator::PlaceRandomRoom( glm::vec2 const& pos )
{
    Opt<IRoom> r;
    mPossibleRooms.ShuffleRoomIds();
    bool succ = false;
    for (auto&& roomId : mPossibleRooms.GetRoomIds())
    {
        auto& room = mRoomRepo( roomId.mRoomId );
        if (mGData.CanPlaceRoom( room.GetRoomDesc(), pos, mPossibleRooms ))
        {
            mGData.PlaceRoom( room.GetRoomDesc(), pos, mPossibleRooms );
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

JungleLevelGenerator::Route_t JungleLevelGenerator::CreateRoute( int32_t startRoomIndex, RouteProperties const& properties )
{
    mGData.ShuffleNeighbours();
    Route_t route;
    int32_t currRoomIndex = startRoomIndex;
    route.push( currRoomIndex );
    Route_t longestRoute;
    Visited_t longestVisited;
    if (std::find( mVisitedRooms.begin(), mVisitedRooms.end(), currRoomIndex ) == mVisitedRooms.end())
    {
        mVisitedRooms.push_back( currRoomIndex );
    }
    std::vector<int32_t> nextNeigh(mGData.GetRoomCount(), 0);
    bool endHit = false;
    while (currRoomIndex != -1 && !endHit)
    {
        while (nextNeigh[currRoomIndex] < mGData.GetNeighbourRoomCount(currRoomIndex)
            &&std::find(mVisitedRooms.begin(), mVisitedRooms.end(),
                mGData.GetNeigbourRoomIndex(currRoomIndex, nextNeigh[currRoomIndex])) != mVisitedRooms.end())
        {
            ++nextNeigh[currRoomIndex];
        }
            
        if(nextNeigh[currRoomIndex] >= mGData.GetNeighbourRoomCount( currRoomIndex ))
        {
            // reached a dead end.
            if (properties.minLength == 0)
            {
                // if minLength is 0 then this means route creation is done
                endHit = true;
            }
            else
            {
                // stepping back
                // TODO: limit step backs in case of too long expected route size, this could be excessive
                nextNeigh[currRoomIndex] = 0;
                if (longestRoute.size() < route.size())
                {
                    // note that longest route and longest visited rooms have to be saved here
                    // the first room (startRoomIndex) have to stay in mVisited
                    longestRoute = route;
                    longestVisited = mVisitedRooms;
                }
                mVisitedRooms.erase( std::find( mVisitedRooms.begin(), mVisitedRooms.end(), currRoomIndex ) );
                route.pop();
                if (route.empty())
                {
                    currRoomIndex = -1;
                }
                else
                {
                    currRoomIndex = route.top();
                    ++nextNeigh[currRoomIndex];
                }
            }
        }
        else
        {
            // searching for a next neighbour
            int32_t nextRoomIndex = mGData.GetNeigbourRoomIndex( currRoomIndex, nextNeigh[currRoomIndex] );
            mVisitedRooms.push_back( nextRoomIndex );
            currRoomIndex = nextRoomIndex;
            route.push( currRoomIndex );
            if ((int32_t)route.size() - properties.minLength>0)
            {
                if (mRand() % (100) < properties.endChance + ((int32_t)route.size() - properties.minLength)*properties.chanceIncrease)
                {
                    // the desired route size has been hit
                    endHit = true;
                }
            }
        }
    }
    if (route.empty())
    {
        std::swap( mVisitedRooms, longestVisited );
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
        auto cellPairs = mGData.GetAdjacentCellPairs( roomA, roomB );
        auto& cellPair = cellPairs[mRand() % cellPairs.size()];
        mGData.LinkCells( cellPair.first, cellPair.second );
    }
}

void JungleLevelGenerator::RouteProperties::Load( Json::Value& setters )
{
    Json::GetInt( setters["min_length"], minLength );
    Json::GetInt( setters["end_chance"], endChance );
    Json::GetInt( setters["chance_increase"], chanceIncrease );
}

} // namespace map

