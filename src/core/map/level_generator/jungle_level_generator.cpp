#include "jungle_level_generator.h"
#include "platform/auto_id.h"
#include "../../i_position_component.h"
#include "random.h"
#include "level_generated_event.h"
#include "platform/settings.h"
#include <chrono>

using platform::AutoId;

namespace map {

JungleLevelGenerator::JungleLevelGenerator( int32_t Id )
    : ILevelGenerator( Id )
    , mActorFactory( ActorFactory::Get() )
    , mRoomRepo( RoomRepo::Get() )
{
    mCellSize = 1000;
    mCellCount = 3;
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
            r.insert( GetCell( pos.x, pos.y ).mGeneratorRoomDescIndex );
        }
    }
    return r;
}

void JungleLevelGenerator::CreateStartAndEnd()
{
    mStartIndex = mRand() % mRoomDescs.size();
    mEndIndex = mRand() % mRoomDescs.size();
    while (mStartIndex == mEndIndex)
    {
        mEndIndex = mRand() % mRoomDescs.size();
    }
    auto& startRoom = mRoomDescs.at( mStartIndex );
    //TODO: check if it has a start property
    startRoom.mRoomDesc.GetProperties().clear();
    startRoom.mRoomDesc.GetProperties().insert( RoomDesc::Start );
    auto& endRoom = mRoomDescs.at( mEndIndex );
    //TODO: check if it has a emd property
    endRoom.mRoomDesc.GetProperties().clear();
    endRoom.mRoomDesc.GetProperties().insert( RoomDesc::End );
}

} // namespace map

