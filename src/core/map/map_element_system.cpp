#include "platform/i_platform.h"
#include "map_element_system.h"

namespace map {

MapElementSystem::MapElementSystem()
    : mScene( Scene::Get() )
{
}


void MapElementSystem::Init()
{
    mMapSystem = MapSystem::Get();
}


void MapElementSystem::Update( double DeltaTime )
{
}


} // namespace map

