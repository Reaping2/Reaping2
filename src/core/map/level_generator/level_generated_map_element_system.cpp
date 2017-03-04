#include "platform/i_platform.h"
#include "level_generated_map_element_system.h"
#include "level_generated_map_element.h"

namespace map {

LevelGeneratedMapElementSystem::LevelGeneratedMapElementSystem()
    : MapElementSystem()
{
}


void LevelGeneratedMapElementSystem::Init()
{
    MapElementSystem::Init();
    mOnLevelGenerated=EventServer<map::LevelGeneratedEvent>::Get().Subscribe( boost::bind( &LevelGeneratedMapElementSystem::OnLevelGenerated, this, _1 ) );
}


void LevelGeneratedMapElementSystem::Update( double DeltaTime )
{
    MapElementSystem::Update( DeltaTime );
    if (mHandleTerrainGeneratedCounter > -1)
    {
        --mHandleTerrainGeneratedCounter;
    }
    if (mHandleTerrainGeneratedCounter==0)
    {
        EventServer<LevelGeneratedEvent>::Get().SendEvent( LevelGeneratedEvent( LevelGeneratedEvent::ActorsSpawned ) );
        mHandleTerrainGeneratedCounter = false;
        L2( "Actors Spawned sent!\n" );
    }
}

void LevelGeneratedMapElementSystem::OnLevelGenerated(map::LevelGeneratedEvent const& Evt)
{
    MapElementListFilter<MapSystem::All> mapElementListFilter( mMapSystem->GetMapElementList(), LevelGeneratedMapElement::GetType_static() );
    for (MapElementListFilter<MapSystem::All>::const_iterator levelGeneratedMapElementIt = mapElementListFilter.begin(), levelGeneratedMapElementE = mapElementListFilter.end(); levelGeneratedMapElementIt != levelGeneratedMapElementE; ++levelGeneratedMapElementIt)
    {
        Opt<LevelGeneratedMapElement> levelGeneratedMapElement( *levelGeneratedMapElementIt );
        if (Evt.mState == LevelGeneratedEvent::TerrainGenerated)
        {
            levelGeneratedMapElement->DoOutputId( LevelGeneratedMapElement::GeneratedNodeId(), 1 );
            mHandleTerrainGeneratedCounter = 4;
        }
        else if (Evt.mState == LevelGeneratedEvent::ActorsSpawned)
        {
            levelGeneratedMapElement->DoOutputId( LevelGeneratedMapElement::ActorsSpawnedNodeId(), 1 );
        }
    }
}


} // namespace map

