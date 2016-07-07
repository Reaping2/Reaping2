#include "hdouble_room1.h"
#include "core/i_position_component.h"
#include "random.h"
#include "room_repo.h"
#include "platform/settings.h"

namespace map {

HDoubleRoom1::HDoubleRoom1( int32_t Id )
    : IRoom( Id )
    , mActorFactory( ActorFactory::Get() )
{
    mRoomDesc.SetCellCount( 2 );
    mRoomDesc.SetCellSize( Settings::Get().GetInt( "generator.cell_size", 1000 ) );
    mRoomDesc.GetCell( 0, 0 ).mPossibleEntrances = { Cell::Top, Cell::Bottom, Cell::Left };
    mRoomDesc.GetCell( 0, 0 ).mFilled = true;
    mRoomDesc.GetCell( 1, 0 ).mPossibleEntrances = { Cell::Top, Cell::Right, Cell::Bottom };
    mRoomDesc.GetCell( 1, 0 ).mFilled = true;
    mRoomDesc.GetProperties() = { RoomDesc::Start,RoomDesc::End };
    mRoomDesc.SetRoom( this );
}

void HDoubleRoom1::Generate( RoomDesc& roomDesc, int32_t x, int32_t y )
{
    std::auto_ptr<Actor> wall;// = mActorFactory( AutoId( "wall" ) );

    auto& simpleRoom1 = RoomRepo::Get()(AutoId( "simple_room1" ));
    {
        RoomDesc tempDesc;
        tempDesc.SetCellCount( 1 );
        tempDesc.SetCellSize( roomDesc.GetCellSize() );
        tempDesc.GetCell( 0, 0 ).mPossibleEntrances = roomDesc.GetCell( 0, 0 ).mPossibleEntrances;
        tempDesc.GetCell( 0, 0 ).mPossibleEntrances.insert( Cell::Right );
        tempDesc.GetCell( 0, 0 ).mFilled = true;
        tempDesc.GetProperties() = roomDesc.GetProperties();
        tempDesc.SetRoom( this );
        simpleRoom1.Generate( tempDesc, x, y );
    }
    {
        RoomDesc tempDesc;
        tempDesc.SetCellCount( 1 );
        tempDesc.SetCellSize( roomDesc.GetCellSize() );
        tempDesc.GetCell( 0, 0 ).mPossibleEntrances = roomDesc.GetCell( 1, 0 ).mPossibleEntrances;
        tempDesc.GetCell( 0, 0 ).mPossibleEntrances.insert( Cell::Left );
        tempDesc.GetCell( 0, 0 ).mFilled = true;
        tempDesc.GetProperties().clear();
        tempDesc.SetRoom( this );
        simpleRoom1.Generate( tempDesc, x + roomDesc.GetCellSize(), y );
    }

    int cellCount = mRoomDesc.GetCellSize() / 100;
    if (cellCount > 3)
    {
        for (int i = 0; i < mRand() % cellCount; ++i)
        {
            wall = mActorFactory( AutoId( "wall_small" ) );
            Opt<IPositionComponent> positionC = wall->Get<IPositionComponent>();
            positionC->SetX( (mRand() % (cellCount * 2 - 4) + 2)*100.0 + x );
            positionC->SetY( (mRand() % (cellCount - 4) + 2)*100.0 + y );
            roomDesc.mPlacedActorGUIDs.push_back( wall->GetGUID() );
            mScene.AddActor( wall.release() );
        }
    }
    if (roomDesc.GetProperties().find( RoomDesc::Start ) != roomDesc.GetProperties().end())
    {
        PlaceSoldierSpawnPoint( roomDesc, cellCount / 2 * 100 + x, cellCount / 2 * 100 + y );
    }
    if (roomDesc.GetProperties().find( RoomDesc::End ) != roomDesc.GetProperties().end())
    {
        PlaceLevelEndPoint( roomDesc, cellCount / 2 * 100 + x, cellCount / 2 * 100 + y );
    }
}

} // namespace map

