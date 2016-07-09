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


IRoom::NeighbourCells_t const& IRoom::GetNeighbourCells() const
{
    return mNeighbourCells;
}


void IRoom::Init()
{

    mRoomDesc.SetRoom( this );
    for (int32_t ry = 0; ry < mRoomDesc.GetCellCount(); ++ry)
    {
        for (int32_t rx = 0; rx < mRoomDesc.GetCellCount(); ++rx)
        {
            if (mRoomDesc.IsFilled(rx,ry))
            {
                AddNeighbourCell( rx - 1, ry );
                AddNeighbourCell( rx + 1, ry );
                AddNeighbourCell( rx, ry - 1 );
                AddNeighbourCell( rx, ry + 1 );
            }
        }
    }
}

void IRoom::AddNeighbourCell( int32_t x, int32_t y )
{
    if (std::find( mNeighbourCells.begin(), mNeighbourCells.end(), glm::vec2( x, y ) ) == mNeighbourCells.end()
        && (!IsInBounds(glm::vec2(x,y)) || !mRoomDesc.IsFilled(x,y)))
    {
        mNeighbourCells.push_back( glm::vec2( x, y ) );
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
