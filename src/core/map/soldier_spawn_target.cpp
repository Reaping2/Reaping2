#include "soldier_spawn_target.h"
#include "spawn_actor_map_element.h"
#include "map_system.h"
#include "map_element_factory.h"
#include "engine/engine.h"
#include "input/mouse.h"
#include "../position_component.h"
#include "editor_target_system.h"
#include "soldier_spawn_point_map_element.h"
#include "editor_soldier_spawn_system.h"
#include "../i_move_component.h"
namespace map {

SoldierSpawnTarget::SoldierSpawnTarget(int32_t Id)
    : ITarget( Id )
    , mCursorId(-1)
    , mActorId( Id )
    , mScene( Scene::Get() )
{

}

SoldierSpawnTarget::SoldierSpawnTarget( int32_t Id, int32_t curosrId )
    : ITarget( Id )
    , mCursorId( curosrId )
    , mActorId( Id )
    , mScene( Scene::Get() )
{

}

void SoldierSpawnTarget::Update( double DeltaTime )
{

}

int32_t SoldierSpawnTarget::GetCursorId() const
{
    return mCursorId;
}

int32_t SoldierSpawnTarget::GetActorId() const
{
    return mActorId;
}

void SoldierSpawnTarget::PutTarget( glm::vec2 position )
{
    std::auto_ptr<MapElement> mapElement( MapElementFactory::Get()( AutoId( "soldier_spawn_point" ) ) );
    Opt<SoldierSpawnPointMapElement> soldierSpawn( static_cast<SoldierSpawnPointMapElement*>( mapElement.get() ) );
    soldierSpawn->SetX( position.x );
    soldierSpawn->SetY( position.y );
    int32_t nextIndex = MapSystem::GetNextUniqueSpawnIndex( "s" );
    soldierSpawn->SetIdentifier( AutoId( "s" + std::to_string( nextIndex ) ) );
    EditorSoldierSpawnSystem::Spawn( soldierSpawn );
    MapSystem::Get()->GetMapElementList().insert( Opt<MapElement>( mapElement.release() ) );
}

std::auto_ptr<Actor> SoldierSpawnTarget::GetCursor()
{
    std::auto_ptr<Actor> player( ActorFactory::Get()( mCursorId ) );
    Opt<IMoveComponent> moveC( player->Get<IMoveComponent>() );
    moveC->SetMoving( false );
    return player;
}

bool SoldierSpawnTarget::Load( const Json::Value& setters )
{
    std::string cursor_id;
    if( !Json::GetStr(setters["cursor_id"], cursor_id))
    {
        L1("Error retrieving 'cursor_id' \n" );
        return false;
    }
    mCursorId = AutoId( cursor_id );
    return true;
}
} // namespace map
