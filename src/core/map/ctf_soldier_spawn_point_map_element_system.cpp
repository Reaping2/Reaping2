#include "platform/i_platform.h"
#include "ctf_soldier_spawn_point_map_element_system.h"
#include "core/map/ctf_soldier_spawn_point_map_element.h"
#include "engine/engine.h"

namespace map {
namespace ctf {

CtfSoldierSpawnPointMapElementSystem::CtfSoldierSpawnPointMapElementSystem()
    : MapElementSystem()
{
}


void CtfSoldierSpawnPointMapElementSystem::Init()
{
    MapElementSystem::Init();
}


void CtfSoldierSpawnPointMapElementSystem::Update( double DeltaTime )
{
    MapElementSystem::Update( DeltaTime );
    //     MapElementListFilter<MapSystem::All> mapElementListFilter(mMapSystem->GetMapElementList(),CtfSoldierSpawnPointMapElement::GetType_static());
    //     for( MapElementListFilter<MapSystem::All>::const_iterator ctfSoldierSpawnPointMapElementIt = mapElementListFilter.begin(), ctfSoldierSpawnPointMapElementE = mapElementListFilter.end(); ctfSoldierSpawnPointMapElementIt != ctfSoldierSpawnPointMapElementE; ++ctfSoldierSpawnPointMapElementIt )
    //     {
    //         Opt<CtfSoldierSpawnPointMapElement> ctfSoldierSpawnPointMapElement(*ctfSoldierSpawnPointMapElementIt);
    //     }
}

SpawnPoints_t CtfSoldierSpawnPointMapElementSystem::GetActiveSpawnPoints( Team::Type team )
{
    SpawnPoints_t r;
    MapElementListFilter<MapSystem::All> mapElementListFilter( mMapSystem->GetMapElementList(), CtfSoldierSpawnPointMapElement::GetType_static() );
    for( MapElementListFilter<MapSystem::All>::const_iterator ctfSoldierSpawnPointMapElementIt = mapElementListFilter.begin(), ctfSoldierSpawnPointMapElementE = mapElementListFilter.end(); ctfSoldierSpawnPointMapElementIt != ctfSoldierSpawnPointMapElementE; ++ctfSoldierSpawnPointMapElementIt )
    {
        Opt<CtfSoldierSpawnPointMapElement> ctfSoldierSpawnPointMapElement( *ctfSoldierSpawnPointMapElementIt );
        if ( ctfSoldierSpawnPointMapElement->GetTeam() == team )
        {
            r.push_back( SpawnPoint( ctfSoldierSpawnPointMapElement->GetX(), ctfSoldierSpawnPointMapElement->GetY() ) );
        }
    }
    return r;
}

Opt<CtfSoldierSpawnPointMapElementSystem> CtfSoldierSpawnPointMapElementSystem::Get()
{
    return engine::Engine::Get().GetSystem<CtfSoldierSpawnPointMapElementSystem>();
}

} // namespace ctf
} // namespace map

