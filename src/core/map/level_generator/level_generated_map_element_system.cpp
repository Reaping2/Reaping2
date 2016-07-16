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
}

void LevelGeneratedMapElementSystem::OnLevelGenerated(map::LevelGeneratedEvent const& Evt)
{
    MapElementListFilter<MapSystem::All> mapElementListFilter( mMapSystem->GetMapElementList(), LevelGeneratedMapElement::GetType_static() );
    for (MapElementListFilter<MapSystem::All>::const_iterator levelGeneratedMapElementIt = mapElementListFilter.begin(), levelGeneratedMapElementE = mapElementListFilter.end(); levelGeneratedMapElementIt != levelGeneratedMapElementE; ++levelGeneratedMapElementIt)
    {
        Opt<LevelGeneratedMapElement> levelGeneratedMapElement( *levelGeneratedMapElementIt );
        levelGeneratedMapElement->DoOutputId( LevelGeneratedMapElement::GeneratedNodeId(), 1 );
    }
}


} // namespace map

