#include "platform/i_platform.h"
#include "soldier_spawn_point_map_element_system.h"
#include "core/map/soldier_spawn_point_map_element.h"
#include "engine/engine.h"

namespace map {

SoldierSpawnPointMapElementSystem::SoldierSpawnPointMapElementSystem()
    : MapElementSystem()
{
}


void SoldierSpawnPointMapElementSystem::Init()
{
    MapElementSystem::Init();
    mOnMapStart = EventServer<core::MapStartEvent>::Get().Subscribe( boost::bind( &SoldierSpawnPointMapElementSystem::OnMapStart, this, _1 ) );
}


void SoldierSpawnPointMapElementSystem::Update( double DeltaTime )
{
    MapElementSystem::Update( DeltaTime );
    //     MapElementListFilter<MapSystem::All> mapElementListFilter(mMapSystem->GetMapElementList(),SoldierSpawnPointMapElement::GetType_static());
    //     for( MapElementListFilter<MapSystem::All>::const_iterator soldierSpawnPointMapElementIt = mapElementListFilter.begin(), soldierSpawnPointMapElementE = mapElementListFilter.end(); soldierSpawnPointMapElementIt != soldierSpawnPointMapElementE; ++soldierSpawnPointMapElementIt )
    //     {
    //         Opt<SoldierSpawnPointMapElement> soldierSpawnPointMapElement(*soldierSpawnPointMapElementIt);
    //     }
}

void SoldierSpawnPointMapElementSystem::OnMapStart( core::MapStartEvent const& Evt )
{
}

Opt<SoldierSpawnPointMapElementSystem> SoldierSpawnPointMapElementSystem::Get()
{
    return engine::Engine::Get().GetSystem<SoldierSpawnPointMapElementSystem>();
}

SpawnPoints_t SoldierSpawnPointMapElementSystem::GetActiveSpawnPoints()
{
    SpawnPoints_t r;
    MapElementListFilter<MapSystem::All> mapElementListFilter( mMapSystem->GetMapElementList(), SoldierSpawnPointMapElement::GetType_static() );
    for( MapElementListFilter<MapSystem::All>::const_iterator soldierSpawnPointMapElementIt = mapElementListFilter.begin(), soldierSpawnPointMapElementE = mapElementListFilter.end(); soldierSpawnPointMapElementIt != soldierSpawnPointMapElementE; ++soldierSpawnPointMapElementIt )
    {
        Opt<SoldierSpawnPointMapElement> soldierSpawnPointMapElement( *soldierSpawnPointMapElementIt );
        r.push_back( SpawnPoint( soldierSpawnPointMapElement->GetX(), soldierSpawnPointMapElement->GetY() ) );
    }
    return r;
}


} // namespace map

