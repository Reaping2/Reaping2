#include "core/map/level_generator/spawn_property.h"
#include "../spawn_actor_map_element.h"
#include "core/position_component.h"
#include "level_generated_map_element.h"


namespace map {

SpawnProperty::SpawnProperty( int32_t Id )
    : IProperty( Id )
    , mTargets()
    , mChance( 100 )
{
}

void SpawnProperty::Load( Json::Value& setters )
{
    IProperty::Load( setters );
    mTargets.clear();
    auto const& targets = setters["targets"];
    if (targets.isArray())
    {
        for (auto& target : targets)
        {
            mTargets.push_back( AutoId( target.asString() ) );
        }
    }
    int32_t chance;
    if (Json::GetInt( setters["chance"], chance ))
    {
        mChance = chance;
    }
}


void SpawnProperty::SetTargets( Targets_t blockedTargets )
{
    mTargets = blockedTargets;
}

SpawnProperty::Targets_t const& SpawnProperty::GetTargets() const
{
    return mTargets;
}



void SpawnProperty::SetChance( int32_t chance )
{
    mChance = chance;
}


int32_t SpawnProperty::GetChance() const
{
    return mChance;
}


void SpawnProperty::Generate( RoomDesc& roomDesc, MapElementHolder mMapElementHolder, glm::vec2 pos )
{
    static int32_t componentId = AutoId( "position_component" );
    for (auto& target : mTargets)
    {
        for (auto targetMapElement : MapElementListFilter<MapSystem::UID>( mMapElementHolder.mAllMapElements, target ))
        {
            Opt<SpawnActorMapElement> spawnActor( targetMapElement );
            if (spawnActor.IsValid())
            {
                Opt<PositionComponentLoader> positionCompLoader = spawnActor->GetComponentLoader( componentId );
                if (positionCompLoader.IsValid())
                {
                    positionCompLoader->Bind<double>( &PositionComponent::AddX, pos.x );
                    positionCompLoader->Bind<double>( &PositionComponent::AddY, pos.y );
                }
                for (Opt<LevelGeneratedMapElement> levelGenerated : MapElementListFilter<MapSystem::All>( MapSystem::Get()->GetMapElementList(), LevelGeneratedMapElement::GetType_static() ))
                {
                    levelGenerated->PlugInNodeId( LevelGeneratedMapElement::GeneratedNodeId(), spawnActor->GetInputNodeId( SpawnActorMapElement::SpawnNodeId() ) );
                }
            }
        }
    }
    for (auto elem : mMapElementHolder.mAllMapElements)
    {
        MapSystem::Get()->GetMapElementList().insert( elem );
    }
//     for (Opt<LevelGeneratedMapElement> levelGenerated : MapElementListFilter<MapSystem::All>( MapSystem::Get()->GetMapElementList(), LevelGeneratedMapElement::GetType_static() ))
//     {
//         MapElementListFilter<MapSystem::All> mapElementListFilter( MapSystem::Get()->GetMapElementList(), SpawnActorMapElement::GetType_static() );
//         for (MapElementListFilter<MapSystem::All>::const_iterator spawnActorMapElementIt = mapElementListFilter.begin(), spawnActorMapElementE = mapElementListFilter.end(); spawnActorMapElementIt != spawnActorMapElementE; ++spawnActorMapElementIt)
//         {
//             Opt<SpawnActorMapElement> spawnActorMapElement( *spawnActorMapElementIt );
// 
//             levelGenerated->PlugInNode( 0, spawnActorMapElement->GetInputNode( 0 ) );
// 
//         }
//     }
}

} // namespace map
