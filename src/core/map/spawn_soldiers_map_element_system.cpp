#include "platform/i_platform.h"
#include "spawn_soldiers_map_element_system.h"
#include "core/map/spawn_soldiers_map_element.h"

namespace map {

SpawnSoldiersMapElementSystem::SpawnSoldiersMapElementSystem()
    : MapElementSystem()
{
}


void SpawnSoldiersMapElementSystem::Init()
{
    MapElementSystem::Init();
}


void SpawnSoldiersMapElementSystem::Update(double DeltaTime)
{
    MapElementSystem::Update(DeltaTime);
    MapElementListFilter<MapSystem::All> mapElementListFilter(mMapSystem->GetMapElementList(),SpawnSoldiersMapElement::GetType_static());
    for( MapElementListFilter<MapSystem::All>::const_iterator spawnSoldiersMapElementIt = mapElementListFilter.begin(), spawnSoldiersMapElementE = mapElementListFilter.end(); spawnSoldiersMapElementIt != spawnSoldiersMapElementE; ++spawnSoldiersMapElementIt )
    {
        Opt<SpawnSoldiersMapElement> spawnSoldiersMapElement(*spawnSoldiersMapElementIt);
        if (spawnSoldiersMapElement->mSpawnInputCounter>0)
        {
            L1("spawn soldiers!");
        }
        spawnSoldiersMapElement->mSpawnInputCounter=0;
    }
}


} // namespace map

