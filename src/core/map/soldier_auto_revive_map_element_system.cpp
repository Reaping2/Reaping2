#include "platform/i_platform.h"
#include "soldier_auto_revive_map_element_system.h"
#include "core/map/soldier_auto_revive_map_element.h"
#include "engine/engine.h"

namespace map {

SoldierAutoReviveMapElementSystem::SoldierAutoReviveMapElementSystem()
    : MapElementSystem()
{
}


void SoldierAutoReviveMapElementSystem::Init()
{
    MapElementSystem::Init();
}


void SoldierAutoReviveMapElementSystem::Update( double DeltaTime )
{
    MapElementSystem::Update( DeltaTime );
}

Opt<SoldierAutoReviveMapElementSystem> SoldierAutoReviveMapElementSystem::Get()
{
    return engine::Engine::Get().GetSystem<SoldierAutoReviveMapElementSystem>();
}

double SoldierAutoReviveMapElementSystem::GetSecsToRevive()
{
    MapElementListFilter<MapSystem::All> mapElementListFilter( mMapSystem->GetMapElementList(), SoldierAutoReviveMapElement::GetType_static() );
    auto it = mapElementListFilter.begin();
    if( it != mapElementListFilter.end() )
    {
        Opt<SoldierAutoReviveMapElement> soldierAutoReviveMapElement( *it );
        return soldierAutoReviveMapElement->GetSecsToRevive();
    }
    return 0.0;
}


} // namespace map

