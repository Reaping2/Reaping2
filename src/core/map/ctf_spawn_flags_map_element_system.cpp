#include "platform/i_platform.h"
#include "ctf_spawn_flags_map_element_system.h"
#include "core/map/ctf_spawn_flags_map_element.h"
#include "ctf_flag_spawn_point_map_element.h"
#include "../i_team_component.h"
#include "../i_position_component.h"
#include "engine/flag_created_event.h"
#include "engine/flag_spawn_system.h"

namespace map {
namespace ctf {

CtfSpawnFlagsMapElementSystem::CtfSpawnFlagsMapElementSystem()
    : MapElementSystem()
    , mProgramState( ::core::ProgramState::Get() )
    , mActorFactory( ActorFactory::Get() )
    , mFlagAutoId( AutoId( "flag" ) )
    , mPlatformAutoId( AutoId( "platform" ) )
{
}


void CtfSpawnFlagsMapElementSystem::Init()
{
    MapElementSystem::Init();
}


void CtfSpawnFlagsMapElementSystem::Update( double DeltaTime )
{
    MapElementSystem::Update( DeltaTime );
    MapElementListFilter<MapSystem::All> mapElementListFilter( mMapSystem->GetMapElementList(), CtfSpawnFlagsMapElement::GetType_static() );
    for( MapElementListFilter<MapSystem::All>::const_iterator ctfSpawnFlagsMapElementIt = mapElementListFilter.begin(), ctfSpawnFlagsMapElementE = mapElementListFilter.end(); ctfSpawnFlagsMapElementIt != ctfSpawnFlagsMapElementE; ++ctfSpawnFlagsMapElementIt )
    {
        Opt<CtfSpawnFlagsMapElement> ctfSpawnFlagsMapElement( *ctfSpawnFlagsMapElementIt );
        if ( ctfSpawnFlagsMapElement->GetValueId( CtfSpawnFlagsMapElement::SpawnNodeId() ) > 0 )
        {
            if ( mProgramState.mMode == core::ProgramState::Server || mProgramState.mMode == core::ProgramState::Local )
            {
                MapElementListFilter<MapSystem::All> mapElementListFilter( mMapSystem->GetMapElementList(), CtfFlagSpawnPointMapElement::GetType_static() );
                for( MapElementListFilter<MapSystem::All>::const_iterator ctfFlagSpawnPointMapElementIt = mapElementListFilter.begin(), ctfFlagSpawnPointMapElementE = mapElementListFilter.end(); ctfFlagSpawnPointMapElementIt != ctfFlagSpawnPointMapElementE; ++ctfFlagSpawnPointMapElementIt )
                {
                    Opt<CtfFlagSpawnPointMapElement> ctfFlagSpawnPointMapElement( *ctfFlagSpawnPointMapElementIt );
                    Spawn( ctfFlagSpawnPointMapElement );

                }
            }
            //             else if (mProgramState.mMode==core::ProgramState::Local)
            //             {
            //                 // no local support for ctf
            //             }
            L2( "spawn flags!" );
        }
        ctfSpawnFlagsMapElement->ResetValues();
    }
}

void CtfSpawnFlagsMapElementSystem::Spawn( Opt<CtfFlagSpawnPointMapElement> ctfFlagSpawnPointMapElement )
{
    std::auto_ptr<Actor> ctfPlatform( ActorFactory::Get()( AutoId( "platform" ) ) );
    Opt<ITeamComponent> teamC( ctfPlatform->Get<ITeamComponent>() );
    if ( teamC.IsValid() )
    {
        teamC->SetTeam( ctfFlagSpawnPointMapElement->GetTeam()/*==Team::Red?Team::Blue:Team::Red*/ );
    }
    ctfFlagSpawnPointMapElement->SetSpawnedActorGUID( ctfPlatform->GetGUID() );
    engine::ctf::FlagSpawnSystem::Get()->SetFlagPositionToStart( *ctfPlatform.get() );
    EventServer<engine::FlagCreatedEvent>::Get().SendEvent( engine::FlagCreatedEvent( Opt<Actor>( ctfPlatform.get() ) ) );
    Scene::Get().AddActor( ctfPlatform.release() );

    std::auto_ptr<Actor> ctfFlag( ActorFactory::Get()( AutoId( "flag" ) ) );
    teamC = ctfFlag->Get<ITeamComponent>();
    if ( teamC.IsValid() )
    {
        teamC->SetTeam( ctfFlagSpawnPointMapElement->GetTeam() );
    }
    engine::ctf::FlagSpawnSystem::Get()->SetFlagPositionToStart( *ctfFlag.get() );
    EventServer<engine::FlagCreatedEvent>::Get().SendEvent( engine::FlagCreatedEvent( Opt<Actor>( ctfFlag.get() ) ) );

    Scene::Get().AddActor( ctfFlag.release() );
}

} // namespace ctf
} // namespace map

