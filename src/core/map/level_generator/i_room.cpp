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

bool IRoom::PlaceRoom( ILevelGenerator& levelGenerator, int32_t x, int32_t y )
{
    L1( "place room\n" );
    for (int32_t ry = 0; ry < mRoomDesc.GetCellCount();++ry)
    {
        for (int32_t rx = 0; rx < mRoomDesc.GetCellCount(); ++rx)
        {
            if (mRoomDesc.GetCells()[ry][rx].mFilled)
            {
                if (!levelGenerator.IsInBounds( rx + x, ry + y ))
                {
                    L1( "%d,%d is out of bounds\n", rx + x, ry + y );
                    return false; //cell is out of bounds
                }
                if (levelGenerator.mCells[ry + y][rx + x].mFilled)
                {
                    L1( "%d,%d is already filled %d\n", rx + x, ry + y, levelGenerator.mCells[ry + y][rx + x].mFilled );
                    return false; //cell is already filled this room cant be placed
                }
            }
        }
    }

    for (int32_t ry = 0; ry < mRoomDesc.GetCellCount(); ++ry)
    {
        for (int32_t rx = 0; rx < mRoomDesc.GetCellCount(); ++rx)
        {
            if (levelGenerator.IsInBounds(rx + x,ry + y))
            {
                auto& cell=levelGenerator.mCells[ry + y][rx + x];
                cell.mDescCoord = glm::vec2( rx, ry );
                cell.mRoomCoord = glm::vec2( x, y );
                cell.mFilled = cell.mFilled||mRoomDesc.GetCells()[ry][rx].mFilled;
                L1( "%d,%d is now filled %d\n", rx + x, ry + y, cell.mFilled );
                cell.mRoomDesc = mRoomDesc;
            }
        }
    }
}


void IRoom::InsertNeighbours( ILevelGenerator& levelGenerator, int32_t x, int32_t y )
{
    for (int32_t ry = 0; ry < mRoomDesc.GetCellCount(); ++ry)
    {
        for (int32_t rx = 0; rx < mRoomDesc.GetCellCount(); ++rx)
        {
            if (mRoomDesc.GetCells()[ry][rx].mFilled)
            {
                InsertNeighbour( levelGenerator, rx + x - 1, ry + y );
                InsertNeighbour( levelGenerator, rx + x + 1, ry + y );
                InsertNeighbour( levelGenerator, rx + x, ry + y - 1 );
                InsertNeighbour( levelGenerator, rx + x, ry + y + 1 );
            }
        }
    }
}

RoomDesc IRoom::GetRoomDesc()const
{
    return mRoomDesc;
}


int32_t IRoom::GetId() const
{
    return mId;
}

void IRoom::InsertNeighbour( ILevelGenerator& levelGenerator, int32_t x, int32_t y )
{
    if (!levelGenerator.IsInBounds(x,y))
    {
        return;
    }
    if (levelGenerator.mCells[y][x].mFilled)
    {
        return;
    }
    L1( "%d,%d not filled %d\n", x, y, levelGenerator.mCells[y][x].mFilled );
    if (std::find(levelGenerator.mFreeNodes.begin(),levelGenerator.mFreeNodes.end(),glm::vec2(x,y))!=levelGenerator.mFreeNodes.end())
    {
        return;
    }
    levelGenerator.mFreeNodes.push_back( glm::vec2(x, y) );
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
