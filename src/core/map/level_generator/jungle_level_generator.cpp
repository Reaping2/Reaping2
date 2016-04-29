#include "jungle_level_generator.h"
#include "platform/auto_id.h"
#include "../../i_position_component.h"

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
    srand(10);
    mCellSize = 1000;
    mCellCount = 4;
    mCells.resize( mCellCount );
    for (auto& row : mCells)
    {
        row.resize( mCellCount );
    }
    int32_t x = 0;// rand() % mCellCount;
    int32_t y = 0;// rand() % mCellCount;
    auto& room = mRoomRepo( AutoId( "simple_room1" ) );
    room.PlaceRoom( *this, x, y );
    GeneratorRoomDesc gRoomDesc;
    gRoomDesc.mRoomCoord = glm::vec2( x, y );
    gRoomDesc.mRoomDesc = room.GetRoomDesc();
    gRoomDesc.mRoomDesc.GetProperties().clear();
    gRoomDesc.mRoomDesc.ClearAllCellentrances();
    mRoomDescs.push_back( gRoomDesc );
    room.InsertNeighbours( *this, x, y );
    //for (int i = 0; i < 17;++i)
    while (!mFreeNodes.empty())
    {
        glm::vec2 vec=mFreeNodes.front();
        auto& room = mRoomRepo( AutoId( "simple_room1" ) );
        room.PlaceRoom( *this, vec.x, vec.y );
        GeneratorRoomDesc gRoomDesc;
        gRoomDesc.mRoomCoord = vec;
        gRoomDesc.mRoomDesc = room.GetRoomDesc();
        gRoomDesc.mRoomDesc.GetProperties().clear();
        gRoomDesc.mRoomDesc.ClearAllCellentrances();
        mRoomDescs.push_back( gRoomDesc );
        L1( "%d pre-pop\n", mFreeNodes.size() );
        for (auto& fn : mFreeNodes)
        {
            L1( "(%f,%f)", fn.x, fn.y );
        }
        L1( "\n" );
        mFreeNodes.pop_front();
        L1( "%d post-pop\n", mFreeNodes.size() );
        for (auto& fn : mFreeNodes)
        {
            L1( "(%f,%f)", fn.x, fn.y );
        }
        L1( "\n" );
        room.InsertNeighbours( *this, vec.x, vec.y );
        L1( "%d after-insert\n", mFreeNodes.size() );
        for (auto& fn : mFreeNodes)
        {
            L1( "(%f,%f)", fn.x, fn.y );
        }
        L1( "\n" );
    }

    for (auto& roomDesc : mRoomDescs)
    {
        roomDesc.mRoomDesc.GetRoom()->Generate( 
            roomDesc.mRoomDesc
            , roomDesc.mRoomCoord.x*mCellSize - 2500
            , roomDesc.mRoomCoord.y*mCellSize - 2500 );
    }

//     auto& room = mRoomRepo( AutoId( "simple_room1" ) );
//     RoomDesc rd;
//     rd.SetCellCount( 1 );
//     rd.GetCells()[0][0].mEntrances.insert( Cell::Left );
//     rd.GetCells()[0][0].mEntrances.insert( Cell::Right );
//     room.Generate(rd);
}

} // namespace map

