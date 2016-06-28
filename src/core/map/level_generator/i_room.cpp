#include "i_room.h"
#include "..\..\i_position_component.h"
#include "..\..\actor_factory.h"

namespace map {

IRoom::IRoom( int32_t Id )
    : mId(Id)
    , mScene(Scene::Get())
{

}

IRoom::~IRoom()
{

}

RoomDesc const& IRoom::GetRoomDesc()const
{
    return mRoomDesc;
}


int32_t IRoom::GetId() const
{
    return mId;
}


IRoom::Neighbours_t const& IRoom::GetNeighbours() const
{
    return mNeighbours;
}


void IRoom::Init()
{

    mRoomDesc.SetRoom( this );
    for (int32_t ry = 0; ry < mRoomDesc.GetCellCount(); ++ry)
    {
        for (int32_t rx = 0; rx < mRoomDesc.GetCellCount(); ++rx)
        {
            if (mRoomDesc.GetCell(rx,ry).mFilled)
            {

                AddNeighbour( rx - 1, ry );
                AddNeighbour( rx + 1, ry );
                AddNeighbour( rx, ry - 1 );
                AddNeighbour( rx, ry + 1 );
            }
        }
    }
}


void IRoom::AddNeighbour( int32_t x, int32_t y )
{
    if (std::find( mNeighbours.begin(), mNeighbours.end(), glm::vec2( x, y ) ) == mNeighbours.end()
        && (!IsInBounds(glm::vec2(x,y)) || !mRoomDesc.GetCell(x,y).mFilled))
    {
        mNeighbours.push_back( glm::vec2( x, y ) );
    }

}

bool IRoom::IsInBounds( glm::vec2 pos ) const
{
    return pos.x >= 0 && pos.y >= 0 && pos.x < mRoomDesc.GetCellCount() && pos.y < mRoomDesc.GetCellCount();
}

void IRoom::PlaceSoldierSpawnPoint( RoomDesc &roomDesc, int32_t x, int32_t y )
{
    std::auto_ptr<MapElement> mapElement( MapElementFactory::Get()(AutoId( "soldier_spawn_point" )) );
    Opt<SoldierSpawnPointMapElement> ctfSoldierSpawn( static_cast<SoldierSpawnPointMapElement*>(mapElement.get()) );
    ctfSoldierSpawn->SetX( x );
    ctfSoldierSpawn->SetY( y );
    MapSystem::Get()->GetMapElementList().insert( Opt<MapElement>( mapElement.release() ) );
}


void IRoom::PlaceLevelEndPoint( RoomDesc &roomDesc, int32_t x, int32_t y )
{
    auto endPoint = ActorFactory::Get()( AutoId( "platform" ) );
    Opt<IPositionComponent> positionC = endPoint->Get<IPositionComponent>();
    positionC->SetX( x );
    positionC->SetY( y );
    mScene.AddActor( endPoint.release() );
}

} // namespace map
