#include "platform/i_platform.h"
#include "editor_soldier_spawn_system.h"
#include "map_system.h"
#include "../i_team_component.h"
#include "../i_position_component.h"
#include "../i_move_component.h"

namespace map {

EditorSoldierSpawnSystem::EditorSoldierSpawnSystem()
    : mScene( Scene::Get() )
    , mActorFactory( ActorFactory::Get() )
{
}


void EditorSoldierSpawnSystem::Init()
{
    mOnMapStart = EventServer<core::MapStartEvent>::Get().Subscribe( boost::bind( &EditorSoldierSpawnSystem::OnMapStart, this, _1 ) );
}


void EditorSoldierSpawnSystem::Update( double DeltaTime )
{
}

void EditorSoldierSpawnSystem::OnMapStart( core::MapStartEvent const& Evt )
{
    if ( mEnabled )
    {
        MapElementListFilter<MapSystem::All> mapElementListFilter( MapSystem::Get()->GetMapElementList(), ctf::CtfSoldierSpawnPointMapElement::GetType_static() );
        for( MapElementListFilter<MapSystem::All>::const_iterator ctfSoldierSpawnPointMapElementIt = mapElementListFilter.begin(), ctfSoldierSpawnPointMapElementE = mapElementListFilter.end(); ctfSoldierSpawnPointMapElementIt != ctfSoldierSpawnPointMapElementE; ++ctfSoldierSpawnPointMapElementIt )
        {
            Opt<ctf::CtfSoldierSpawnPointMapElement> ctfSoldierSpawnPointMapElement( *ctfSoldierSpawnPointMapElementIt );
            Spawn( ctfSoldierSpawnPointMapElement );

        }
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


} // namespace map

