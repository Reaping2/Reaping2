#include "vdouble_room1.h"
#include "core/i_position_component.h"
#include "random.h"
#include "room_repo.h"

namespace map {

VDoubleRoom1::VDoubleRoom1( int32_t Id )
    : IRoom( Id )
    , mActorFactory( ActorFactory::Get() )
{
    mRoomDesc.SetCellCount( 2 );
    mRoomDesc.SetCellSize( 1000 );
    mRoomDesc.GetCell( 0, 0 ).mPossibleEntrances = { Cell::Right, Cell::Top, Cell::Left };
    mRoomDesc.GetCell( 0, 0 ).mFilled = true;
    mRoomDesc.GetCell( 0, 1 ).mPossibleEntrances = { Cell::Bottom, Cell::Right, Cell::Left };
    mRoomDesc.GetCell( 0, 1 ).mFilled = true;
    mRoomDesc.GetProperties() = { RoomDesc::Start,RoomDesc::End };
    mRoomDesc.SetRoom( this );
}

void VDoubleRoom1::Generate( RoomDesc& roomDesc, int32_t x, int32_t y )
{
    std::auto_ptr<Actor> wall;// = mActorFactory( AutoId( "wall" ) );

    {
        auto& simpleRoom1 = RoomRepo::Get()(AutoId( "simple_room1" ));
        RoomDesc tempDesc;
        tempDesc.SetCellCount( 1 );
        tempDesc.SetCellSize( roomDesc.GetCellSize() );
        tempDesc.GetCell( 0, 0 ).mPossibleEntrances = roomDesc.GetCell( 0, 0 ).mPossibleEntrances;
        tempDesc.GetCell( 0, 0 ).mPossibleEntrances.insert( Cell::Top );
        tempDesc.GetCell( 0, 0 ).mFilled = true;
        tempDesc.GetProperties() = roomDesc.GetProperties();
        tempDesc.SetRoom( this );
        simpleRoom1.Generate( tempDesc, x, y );
    }
    {
        auto& simpleRoom1 = RoomRepo::Get()(AutoId( "simple_room1" ));
        RoomDesc tempDesc;
        tempDesc.SetCellCount( 1 );
        tempDesc.SetCellSize( roomDesc.GetCellSize() );
        tempDesc.GetCell( 0, 0 ).mPossibleEntrances = roomDesc.GetCell( 0, 1 ).mPossibleEntrances;
        tempDesc.GetCell( 0, 0 ).mPossibleEntrances.insert( Cell::Bottom );
        tempDesc.GetCell( 0, 0 ).mFilled = true;
        tempDesc.GetProperties().clear();
        tempDesc.SetRoom( this );
        simpleRoom1.Generate( tempDesc, x, y + roomDesc.GetCellSize() );
    }
    int cellCount = mRoomDesc.GetCellSize() / 100;

    for (int i = 0; i < 10; ++i)
    {
        wall = mActorFactory( AutoId( "wall_small" ) );
        Opt<IPositionComponent> positionC = wall->Get<IPositionComponent>();
        positionC->SetX( (mRand() % (cellCount - 2) + 1)*100.0 + x );
        positionC->SetY( (mRand() % (cellCount*2 - 2) + 1)*100.0 + y );
        roomDesc.mPlacedActorGUIDs.push_back( wall->GetGUID() );
        mScene.AddActor( wall.release() );
    }
    if (roomDesc.GetProperties().find( RoomDesc::Start ) != roomDesc.GetProperties().end())
    {
        PlaceSoldierSpawnPoint( roomDesc, x + 400, y + 200 );
    }
    if (roomDesc.GetProperties().find( RoomDesc::End ) != roomDesc.GetProperties().end())
    {
        PlaceLevelEndPoint( roomDesc, x + 400, y + 300 );
    }

}

} // namespace map

