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
        if (mCells[vec.y][vec.x].mFilled)
        {
            LogNodes( "filled so pre_pop" ); mFreeNodes.pop_front(); LogNodes( "filled so post_pop" );
            continue;
        }
        Opt<IRoom> placedRoom = PlaceARoom( vec );
        if (placedRoom.IsValid())
        {
            LogNodes( "pre_pop" ); mFreeNodes.pop_front(); LogNodes( "post_pop" );
            if (placedRoom->GetId() == AutoId( "vdouble_room1" ))
            {
                L1( "double room\n" );
            }
            placedRoom->InsertNeighbours( *this, vec.x, vec.y ); LogNodes( "after_insert" );
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
        if (room.PlaceRoom( *this, vec.x, vec.y ))
        {
            GeneratorRoomDesc gRoomDesc;
            gRoomDesc.mRoomCoord = vec;
            gRoomDesc.mRoomDesc = room.GetRoomDesc();
            gRoomDesc.mRoomDesc.GetProperties().clear();
            gRoomDesc.mRoomDesc.ClearAllCellEntrances();
            mRoomDescs.push_back( gRoomDesc );
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
    auto& neighbourRooms = GetNeighbourRooms( mStartIndex );
}


JungleLevelGenerator::NeighbourRooms_t JungleLevelGenerator::GetNeighbourRooms( int32_t roomIndex )
{
    auto& room = mRoomDescs[roomIndex];
    //room.mRoomCoord
    return {};
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

