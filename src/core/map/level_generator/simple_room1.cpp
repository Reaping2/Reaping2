#include "simple_room1.h"
#include "core/i_position_component.h"

namespace map {

SimpleRoom1::SimpleRoom1( int32_t Id )
    : IRoom( Id )
    , mActorFactory( ActorFactory::Get() )
{
    mRoomDesc.SetCellCount( 1 );
    mRoomDesc.SetCellSize( 1000 );
    mRoomDesc.GetCells()[0][0].mEntrances = { Cell::Top, Cell::Right, Cell::Bottom, Cell::Left };
    mRoomDesc.GetCells()[0][0].mFilled = true;
    mRoomDesc.GetProperties() = { RoomDesc::Start,RoomDesc::End };
    mRoomDesc.SetRoom( this );
}

void SimpleRoom1::Generate( RoomDesc& roomDesc, int32_t x, int32_t y )
{
    std::auto_ptr<Actor> wall;// = mActorFactory( AutoId( "wall" ) );

    int cellCount = mRoomDesc.GetCellSize() / 100;
    for (int i = 0; i < cellCount*cellCount; ++i)
    {
        auto& entrances = roomDesc.GetCells()[0][0].mEntrances;
        if (i < cellCount && entrances.find( Cell::Bottom ) == entrances.end()
            || i >= (cellCount - 1)*cellCount && entrances.find( Cell::Top ) == entrances.end()
            || i%cellCount == 0 && entrances.find( Cell::Left ) == entrances.end()
            || i%cellCount == cellCount - 1 && entrances.find( Cell::Right ) == entrances.end())
        {
            wall = mActorFactory( AutoId( "wall_small" ) );
            Opt<IPositionComponent> positionC = wall->Get<IPositionComponent>();
            positionC->SetX( i%cellCount*100.0 + x );
            positionC->SetY( i / cellCount*100.0 + y );
            mScene.AddActor( wall.release() );
        }
    }
}

} // namespace map

