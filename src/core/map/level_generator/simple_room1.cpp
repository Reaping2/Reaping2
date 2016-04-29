#include "simple_room1.h"
#include "core/i_position_component.h"

namespace map {

SimpleRoom1::SimpleRoom1( int32_t Id )
    : IRoom( Id )
    , mActorFactory( ActorFactory::Get() )
{
    mRoomDesc.SetCellCount( 1 );
    mRoomDesc.SetCellSize( 1000 );
    Cell::Entrances_t entrances{ Cell::Top, Cell::Right, Cell::Bottom, Cell::Left };
    mRoomDesc.GetCells()[0][0].mEntrances = entrances;
    RoomDesc::Properties_t properties{ RoomDesc::Start,RoomDesc::End };
    mRoomDesc.SetProperties( properties );
}

void SimpleRoom1::Generate( RoomDesc const& roomDesc )
{
    std::auto_ptr<Actor> wall;// = mActorFactory( AutoId( "wall" ) );

    int cellCount=mRoomDesc.GetCellSize() / 100;
    for (int i = 0; i < cellCount*cellCount; ++i)
    {
        auto& entrances=mRoomDesc.GetCells()[0][0].mEntrances;
        if (i < cellCount&&entrances.find( Cell::Bottom ) == entrances.end()
            || i >( cellCount - 1 )*cellCount
            || i%cellCount == 0 || i%cellCount == cellCount - 1)
        {
            wall = mActorFactory( AutoId( "wall_small" ) );
            Opt<IPositionComponent> positionC = wall->Get<IPositionComponent>();
            positionC->SetX( i%cellCount*100.0-500 );
            positionC->SetY( i / cellCount*100.0-500 );
            mScene.AddActor( wall.release() );
        }
    }
}

} // namespace map

