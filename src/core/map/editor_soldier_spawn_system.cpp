#include "platform/i_platform.h"
#include "editor_soldier_spawn_system.h"
#include "map_system.h"
#include "../i_team_component.h"
#include "../i_position_component.h"
#include "../i_move_component.h"
#include "soldier_spawn_point_map_element.h"

namespace map {

EditorSoldierSpawnSystem::EditorSoldierSpawnSystem()
    : mScene( Scene::Get() )
    , mActorFactory( ActorFactory::Get() )
{
}


void EditorSoldierSpawnSystem::Init()
{
    mOnMapStart = EventServer<core::MapStartEvent>::Get().Subscribe( boost::bind( &EditorSoldierSpawnSystem::OnMapStart, this, _1 ) );
    mOnLevelGenerated = EventServer<LevelGeneratedEvent>::Get().Subscribe( boost::bind( &EditorSoldierSpawnSystem::OnLevelGenerated, this, _1 ) );
}


void EditorSoldierSpawnSystem::Update( double DeltaTime )
{
}

void EditorSoldierSpawnSystem::OnMapStart( core::MapStartEvent const& Evt )
{
    if (Evt.mState == core::MapStartEvent::Started)
    {
        SpawnPlayers();
    }
}

void EditorSoldierSpawnSystem::Spawn( Opt<ctf::CtfSoldierSpawnPointMapElement> ctfSoldierSpawnPointMapElement )
{
    std::auto_ptr<Actor> player( ActorFactory::Get()( AutoId( "ctf_player" ) ) );

    Opt<ITeamComponent> teamC( player->Get<ITeamComponent>() );
    if ( teamC.IsValid() )
    {
        teamC->SetTeam( ctfSoldierSpawnPointMapElement->GetTeam() );
    }
    Opt<IPositionComponent> positionC = player->Get<IPositionComponent>();
    positionC->SetX( ctfSoldierSpawnPointMapElement->GetX() );
    positionC->SetY( ctfSoldierSpawnPointMapElement->GetY() );
    Opt<IMoveComponent> moveC( player->Get<IMoveComponent>() );
    moveC->SetMoving( false );
    ctfSoldierSpawnPointMapElement->SetSpawnedActorGUID( player->GetGUID() );
    Scene::Get().AddActor( player.release() );
}

void EditorSoldierSpawnSystem::Spawn( Opt<SoldierSpawnPointMapElement> soldierSpawnPointMapElement )
{
    std::auto_ptr<Actor> player( ActorFactory::Get()(AutoId( "player" )) );

    Opt<IPositionComponent> positionC = player->Get<IPositionComponent>();
    positionC->SetX( soldierSpawnPointMapElement->GetX() );
    positionC->SetY( soldierSpawnPointMapElement->GetY() );
    Opt<IMoveComponent> moveC( player->Get<IMoveComponent>() );
    moveC->SetMoving( false );
    soldierSpawnPointMapElement->SetSpawnedActorGUID( player->GetGUID() );
    Scene::Get().AddActor( player.release() );
}

void EditorSoldierSpawnSystem::OnLevelGenerated( LevelGeneratedEvent const& Evt )
{
    if (Evt.mState == LevelGeneratedEvent::ActorsSpawned)
    {
        SpawnPlayers();
    }
}

void EditorSoldierSpawnSystem::SpawnPlayers()
{
    if (mEnabled)
    {
        for (Opt<ctf::CtfSoldierSpawnPointMapElement> ctfSoldierSpawnPointMapElement : MapElementListFilter<MapSystem::All>( MapSystem::Get()->GetMapElementList(), ctf::CtfSoldierSpawnPointMapElement::GetType_static() ))
        {
            Spawn( ctfSoldierSpawnPointMapElement );
        }
        for (Opt<SoldierSpawnPointMapElement> soldierSpawnPointMapElement : MapElementListFilter<MapSystem::All>( MapSystem::Get()->GetMapElementList(), SoldierSpawnPointMapElement::GetType_static() ))
        {
            Spawn( soldierSpawnPointMapElement );
        }
    }
}

} // namespace map

