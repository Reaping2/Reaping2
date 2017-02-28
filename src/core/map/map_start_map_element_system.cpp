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
    for (Opt<MapStartMapElement> mapStartMapElement : MapElementListFilter<MapSystem::All>( mMapSystem->GetMapElementList(), MapStartMapElement::GetType_static() ))
    {
        if (mapStartMapElement->GetValueId( MapStartMapElement::StartInputNodeId() ) > 0)
        {
            L2( "MapStart element started\n" );
            EventServer<core::MapStartEvent>::Get().SendEvent( core::MapStartEvent( core::MapStartEvent::Started ) );
            mapStartMapElement->ResetValueId( MapStartMapElement::StartInputNodeId() );
        }
        else if (mapStartMapElement->GetValueId( MapStartMapElement::PreActorsSpawnedInputNodeId() ) > 0)
        {
            L2( "MapStart element pre_actors_spawned\n" );
            EventServer<core::MapStartEvent>::Get().SendEvent( core::MapStartEvent( core::MapStartEvent::PreActorsSpawned ) );
            mapStartMapElement->ResetValueId( MapStartMapElement::PreActorsSpawnedInputNodeId() );
        }
        else if (mapStartMapElement->GetValueId( MapStartMapElement::ActorsSpawnedInputNodeId() ) > 0)
        {
            L2( "MapStart element actors_spawned\n" );
            EventServer<core::MapStartEvent>::Get().SendEvent( core::MapStartEvent( core::MapStartEvent::ActorsSpawned ) );
            mapStartMapElement->ResetValueId( MapStartMapElement::ActorsSpawnedInputNodeId() );
        }
        else if (mapStartMapElement->GetValueId( MapStartMapElement::ReadyInputNodeId() ) > 0)
        {
            L2( "MapStart element ready\n" );
            EventServer<core::MapStartEvent>::Get().SendEvent( core::MapStartEvent( core::MapStartEvent::Ready ) );
            mapStartMapElement->ResetValueId( MapStartMapElement::ReadyInputNodeId() );
        }
    }
}

void MapStartMapElementSystem::OnMapStart( core::MapStartEvent const& Evt )
{
    MapElementListFilter<MapSystem::All> mapElementListFilter( mMapSystem->GetMapElementList(), MapStartMapElement::GetType_static() );
    for( MapElementListFilter<MapSystem::All>::const_iterator mapStartMapElementIt = mapElementListFilter.begin(), mapStartMapElementE = mapElementListFilter.end(); mapStartMapElementIt != mapStartMapElementE; ++mapStartMapElementIt )
    {
        Opt<MapStartMapElement> mapStartMapElement( *mapStartMapElementIt );
        if (Evt.mState == core::MapStartEvent::Started)
        {
            mapStartMapElement->DoOutputId( MapStartMapElement::StartOutputNodeId(), 1 );
        }
        else if (Evt.mState == core::MapStartEvent::PreActorsSpawned)
        {
            mapStartMapElement->DoOutputId( MapStartMapElement::PreActorsSpawnedOutputNodeId(), 1 );
        }
        else if (Evt.mState == core::MapStartEvent::ActorsSpawned)
        {
            mapStartMapElement->DoOutputId( MapStartMapElement::ActorsSpawnedOutputNodeId(), 1 );
        }
        else if (Evt.mState == core::MapStartEvent::Ready)
        {
            mapStartMapElement->DoOutputId( MapStartMapElement::ReadyOutputNodeId(), 1 );
        }
    }
}


} // namespace map

