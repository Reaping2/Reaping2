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
    std::auto_ptr<Actor> wall = mActorFactory(AutoId( "wall" ));

    Opt<IPositionComponent> positionC = wall->Get<IPositionComponent>();
    positionC->SetX( -240 );
    positionC->SetY( 0.0 );
    mScene.AddActor( wall.release() );
    wall = mActorFactory( AutoId( "wall" ) );

    positionC = wall->Get<IPositionComponent>();
    positionC->SetX( 240 );
    positionC->SetY( 0.0 );
    mScene.AddActor( wall.release() );
    auto& room = mRoomRepo( AutoId( "simple_room1" ) );
    RoomDesc rd;
    rd.SetCellCount( 1 );
    rd.GetCells()[0][0].mEntrances.insert( Cell::Bottom );
    room.Generate(rd);
}

} // namespace map

