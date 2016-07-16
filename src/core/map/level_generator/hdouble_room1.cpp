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
    mRoomDesc.GetCell( 0, 0 ).SetEntrances( { EntranceType::Top, EntranceType::Bottom, EntranceType::Left } );
    mRoomDesc.GetCell( 0, 0 ).SetFilled( true );
    mRoomDesc.GetCell( 1, 0 ).SetEntrances( { EntranceType::Top, EntranceType::Right, EntranceType::Bottom } );
    mRoomDesc.GetCell( 1, 0 ).SetFilled( true );
    mRoomDesc.SetProperties( { RoomProperty::Start, RoomProperty::End } );
    mRoomDesc.SetRoom( this );
}

void HDoubleRoom1::Generate( RoomDesc& roomDesc, glm::vec2 pos )
{
    auto& simpleRoom1 = RoomRepo::Get()(AutoId( "simple_room1" ));
    {
        RoomDesc tempDesc;
        tempDesc.SetCellCount( 1 );
        tempDesc.SetCellSize( roomDesc.GetCellSize() );
        tempDesc.GetCell( 0, 0 ).SetEntrances( roomDesc.GetCell( 0, 0 ).GetEntrances() );
        tempDesc.GetCell( 0, 0 ).AddEntrance( EntranceType::Right );
        tempDesc.GetCell( 0, 0 ).SetFilled( true );
        tempDesc.SetProperties( roomDesc.GetProperties() );
        tempDesc.SetRoom( this );
        simpleRoom1.Generate( tempDesc, pos );
    }
    {
        RoomDesc tempDesc;
        tempDesc.SetCellCount( 1 );
        tempDesc.SetCellSize( roomDesc.GetCellSize() );
        tempDesc.GetCell( 0, 0 ).SetEntrances( roomDesc.GetCell( 1, 0 ).GetEntrances() );
        tempDesc.GetCell( 0, 0 ).AddEntrance( EntranceType::Left );
        tempDesc.GetCell( 0, 0 ).SetFilled( true );
        tempDesc.ClearProperties();
        tempDesc.SetRoom( this );
        simpleRoom1.Generate( tempDesc, pos+glm::vec2(roomDesc.GetCellSize(), 0) );
    }

    int cellCount = mRoomDesc.GetCellSize() / 100;
    if (cellCount > 3)
    {
        for (int i = 0; i < mRand() % cellCount; ++i)
        {
            std::auto_ptr<Actor> wall = mActorFactory( AutoId( "wall_small" ) );
            Opt<IPositionComponent> positionC = wall->Get<IPositionComponent>();
            positionC->SetX( (mRand() % (cellCount * 2 - 4) + 2)*100.0 + pos.x );
            positionC->SetY( (mRand() % (cellCount - 4) + 2)*100.0 + pos.y );
            roomDesc.mPlacedActorGUIDs.push_back( wall->GetGUID() );
            mScene.AddActor( wall.release() );
        }
    }
}

} // namespace map

