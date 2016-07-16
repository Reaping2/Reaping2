#include "vdouble_room1.h"
#include "core/i_position_component.h"
#include "random.h"
#include "room_repo.h"
#include "platform/settings.h"

namespace map {

VDoubleRoom1::VDoubleRoom1( int32_t Id )
    : IRoom( Id )
    , mActorFactory( ActorFactory::Get() )
{
    mRoomDesc.SetCellCount( 2 );
    mRoomDesc.SetCellSize( Settings::Get().GetInt( "generator.cell_size", 1000 ) );
    mRoomDesc.GetCell( 0, 0 ).SetEntrances( { EntranceType::Right, EntranceType::Top, EntranceType::Left } );
    mRoomDesc.GetCell( 0, 0 ).SetFilled( true );
    mRoomDesc.GetCell( 0, 1 ).SetEntrances( { EntranceType::Bottom, EntranceType::Right, EntranceType::Left } );
    mRoomDesc.GetCell( 0, 1 ).SetFilled( true );
    mRoomDesc.SetProperties( { RoomProperty::Start, RoomProperty::End } );
    mRoomDesc.SetRoom( this );
}

void VDoubleRoom1::Generate( RoomDesc& roomDesc, glm::vec2 pos )
{
    {
        auto& simpleRoom1 = RoomRepo::Get()(AutoId( "simple_room1" ));
        RoomDesc tempDesc;
        tempDesc.SetCellCount( 1 );
        tempDesc.SetCellSize( roomDesc.GetCellSize() );
        tempDesc.GetCell( 0, 0 ).SetEntrances( roomDesc.GetCell( 0, 0 ).GetEntrances() );
        tempDesc.GetCell( 0, 0 ).AddEntrance( EntranceType::Top );
        tempDesc.GetCell( 0, 0 ).SetFilled( true );
        tempDesc.SetProperties( roomDesc.GetProperties() );
        tempDesc.SetRoom( this );
        simpleRoom1.Generate( tempDesc, pos );
    }
    {
        auto& simpleRoom1 = RoomRepo::Get()(AutoId( "simple_room1" ));
        RoomDesc tempDesc;
        tempDesc.SetCellCount( 1 );
        tempDesc.SetCellSize( roomDesc.GetCellSize() );
        tempDesc.GetCell( 0, 0 ).SetEntrances( roomDesc.GetCell( 0, 1 ).GetEntrances() );
        tempDesc.GetCell( 0, 0 ).AddEntrance( EntranceType::Bottom );
        tempDesc.GetCell( 0, 0 ).SetFilled( true );
        tempDesc.ClearProperties();
        tempDesc.SetRoom( this );
        simpleRoom1.Generate( tempDesc, pos + glm::vec2( 0, roomDesc.GetCellSize() ) );
    }
    int cellCount = mRoomDesc.GetCellSize() / 100;

    if (cellCount > 3)
    {
        for (int i = 0; i < mRand() % cellCount; ++i)
        {
            std::auto_ptr<Actor> wall = mActorFactory( AutoId( "wall_small" ) );
            Opt<IPositionComponent> positionC = wall->Get<IPositionComponent>();
            positionC->SetX( (mRand() % (cellCount - 4) + 2)*100.0 + pos.x );
            positionC->SetY( (mRand() % (cellCount * 2 - 4) + 2)*100.0 + pos.y );
            roomDesc.mPlacedActorGUIDs.push_back( wall->GetGUID() );
            mScene.AddActor( wall.release() );
        }
    }
}

} // namespace map

