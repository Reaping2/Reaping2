#include "platform/i_platform.h"
#include "map_start_map_element_system.h"
#include "core/map/map_start_map_element.h"

namespace map {

MapStartMapElementSystem::MapStartMapElementSystem()
    : MapElementSystem()
{
}


void MapStartMapElementSystem::Init()
{
    MapElementSystem::Init();
    mOnMapStart = EventServer<core::MapStartEvent>::Get().Subscribe( boost::bind( &MapStartMapElementSystem::OnMapStart, this, _1 ) );
}


void MapStartMapElementSystem::Update( double DeltaTime )
{
    MapElementSystem::Update( DeltaTime );
    //     MapElementListFilter<MapSystem::All> mapElementListFilter(mMapSystem->GetMapElementList(),MapStartMapElement::GetType_static());
    //     for( MapElementListFilter<MapSystem::All>::const_iterator mapStartMapElementIt = mapElementListFilter.begin(), mapStartMapElementE = mapElementListFilter.end(); mapStartMapElementIt != mapStartMapElementE; ++mapStartMapElementIt )
    //     {
    //         Opt<MapStartMapElement> mapStartMapElement(*mapStartMapElementIt);
    //     }
}

void MapStartMapElementSystem::OnMapStart( core::MapStartEvent const& Evt )
{
    MapElementListFilter<MapSystem::All> mapElementListFilter( mMapSystem->GetMapElementList(), MapStartMapElement::GetType_static() );
    for( MapElementListFilter<MapSystem::All>::const_iterator mapStartMapElementIt = mapElementListFilter.begin(), mapStartMapElementE = mapElementListFilter.end(); mapStartMapElementIt != mapStartMapElementE; ++mapStartMapElementIt )
    {
        Opt<MapStartMapElement> mapStartMapElement( *mapStartMapElementIt );
        mapStartMapElement->DoOutputId( MapStartMapElement::StartNodeId(), 1 );
    }
}


} // namespace map

