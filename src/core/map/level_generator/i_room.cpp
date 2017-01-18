#include "i_room.h"
#include "core/i_position_component.h"
#include "core/actor_factory.h"

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
                AddNeighbourCell( glm::vec2( rx - 1, ry ) );
                AddNeighbourCell( glm::vec2( rx + 1, ry ) );
                AddNeighbourCell( glm::vec2( rx, ry - 1 ) );
                AddNeighbourCell( glm::vec2( rx, ry + 1 ) );
            }
        }
    }
}


IRoom::Properties_t const& IRoom::GetProperties() const
{
    return mProperties;
}


IRoom::Properties_t& IRoom::GetProperties()
{
    return mProperties;
}


Opt<IProperty> IRoom::GetProperty( int32_t propUID )
{
    for (auto&& prop : mProperties)
    {
        if (propUID == prop.GetUID())
        {
            return &prop;
        }
    }
    return Opt<IProperty>();
}

void IRoom::AddProperty( std::auto_ptr<IProperty> prop )
{
    mProperties.push_back( prop );
}


void IRoom::_SetRoomDesc( RoomDesc const& roomDesc )
{
    mRoomDesc = roomDesc;
}

void IRoom::AddNeighbourCell( glm::vec2 pos )
{
    if (std::find( mNeighbourCells.begin(), mNeighbourCells.end(), pos ) == mNeighbourCells.end()
        && (!IsInBounds(pos) || !mRoomDesc.IsFilled(pos)))
    {
        mNeighbourCells.push_back( pos );
    }

}

bool IRoom::IsInBounds( glm::vec2 pos ) const
{
    return pos.x >= 0 && pos.y >= 0 && pos.x < mRoomDesc.GetCellCount() && pos.y < mRoomDesc.GetCellCount();
}

void IRoom::PlaceSoldierSpawnPoint( RoomDesc &roomDesc, glm::vec2 pos )
{
    std::auto_ptr<MapElement> mapElement( MapElementFactory::Get()(AutoId( "soldier_spawn_point" )) );
    Opt<SoldierSpawnPointMapElement> ctfSoldierSpawn( static_cast<SoldierSpawnPointMapElement*>(mapElement.get()) );
    ctfSoldierSpawn->SetX( pos.x );
    ctfSoldierSpawn->SetY( pos.y );
    MapSystem::Get()->GetMapElementList().insert( Opt<MapElement>( mapElement.release() ) );
}


void IRoom::PlaceLevelEndPoint( RoomDesc &roomDesc, glm::vec2 pos )
{
    auto endPoint = ActorFactory::Get()( AutoId( "platform" ) );
    Opt<IPositionComponent> positionC = endPoint->Get<IPositionComponent>();
    positionC->SetX( pos.x );
    positionC->SetY( pos.y );
    mScene.AddActor( endPoint.release() );
}

} // namespace map
