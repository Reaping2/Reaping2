#include "core/map/level_generator/spawn_property.h"
#include "../spawn_actor_map_element.h"
#include "core/position_component.h"
#include "level_generated_map_element.h"
#include "random.h"
#include "../group_map_element.h"
#include "../respawn_actor_map_element.h"


namespace map {

SpawnProperty::SpawnProperty( int32_t Id )
    : IProperty( Id )
    , mTargets()
    , mChance( 100 )
{
}

void SpawnProperty::Load( Json::Value const& setters )
{
    IProperty::Load( setters );
    mTargets.clear();
    auto const& targets = setters["targets"];
    if (targets.isArray())
    {
        for (auto& target : targets)
        {
            mTargets.push_back( AutoId( target.asString() ) );
            L2( "Targets:%s\n ", target.asString().c_str() );
        }
    }
    int32_t chance;
    if (Json::GetInt( setters["chance"], chance ))
    {
        L2( "Chance: %d\n", chance );
        mChance = chance;
    }
    L2( "Chance end\n");
}


void SpawnProperty::Save( Json::Value& setters ) const
{
    IProperty::Save( setters );
    auto& idStorage = IdStorage::Get();
    Json::Value TargetArr( Json::arrayValue );
    for (auto& target : mTargets)
    {
        std::string targetName;
        if (idStorage.GetName( target, targetName ))
        {
            Json::Value jName = Json::Value( targetName );
            TargetArr.append( jName );
        }
    }
    setters["targets"] = TargetArr;
    setters["chance"] = Json::Value( mChance );
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


void SpawnProperty::Generate( RoomDesc& roomDesc, MapElementHolder& mMapElementHolder, glm::vec2 pos, bool editor /*= false*/ )
{
    int32_t const ran = mRand() % 100;
    if (ran < mChance || editor)
    {
        auto& idStorage = IdStorage::Get();
        Json::Value TargetArr( Json::arrayValue );
        for (auto& target : mTargets)
        {
            std::string targetName;
            if (idStorage.GetName( target, targetName ))
            {
                Json::Value jName = Json::Value( targetName );
                L2("Target:%s\n", targetName.c_str() );
            }
        }
        L2( "Spawn by chance: %d, %d \n", ran, mChance );
        SpawnTargets( roomDesc, mTargets, mMapElementHolder, pos );
    }
}


void SpawnProperty::SpawnTargets( RoomDesc &roomDesc, std::vector<int32_t> targets, MapElementHolder &mMapElementHolder, glm::vec2 &pos )
{
    static Opt<MapSystem> mapSystem = MapSystem::Get();
    int currTargetIndex = 0;
    while (currTargetIndex<targets.size())
    {
        int32_t target = targets[currTargetIndex];
        for (auto targetMapElement : MapElementListFilter<MapSystem::Identifier>( mMapElementHolder.mAllMapElements, target ))
        {
            if (targetMapElement->GetType() == SpawnActorMapElement::GetType_static())
            {
                Opt<SpawnActorMapElement> spawnActorMapElement( Opt<map::MapElement>( targetMapElement->clone() ) );
                SpawnActor( spawnActorMapElement, pos );
                mapSystem->GetMapElementList().insert( spawnActorMapElement );
            }
            else if (targetMapElement->GetType() == RespawnActorMapElement::GetType_static())
            {
                Opt<RespawnActorMapElement> respawnActorMapElement( Opt<map::MapElement>( targetMapElement->clone() ) );
                RespawnActor( respawnActorMapElement, pos );
                mapSystem->GetMapElementList().insert( respawnActorMapElement );
            }
            else if (targetMapElement->GetType() == GroupMapElement::GetType_static())
            {
                Opt<GroupMapElement> groupMapElement( Opt<map::MapElement>( targetMapElement->clone() ) );
                targets.insert( targets.end(), groupMapElement->GetTargets().begin(), groupMapElement->GetTargets().end() );
                mapSystem->GetMapElementList().insert( groupMapElement );
            }
        }
        ++currTargetIndex;
    }
}


void SpawnProperty::SpawnActor( Opt<SpawnActorMapElement> spawnActorMapElement, glm::vec2 &pos )
{
    static int32_t componentId = AutoId( "position_component" );
    static Opt<MapSystem> mapSystem = MapSystem::Get();
    Opt<PositionComponentLoader> positionCompLoader = spawnActorMapElement->GetComponentLoader( componentId );
    if (positionCompLoader.IsValid())
    {
        positionCompLoader->Bind<double>( &PositionComponent::AddX, pos.x );
        positionCompLoader->Bind<double>( &PositionComponent::AddY, pos.y );
    }
    for (Opt<LevelGeneratedMapElement> levelGenerated : MapElementListFilter<MapSystem::All>( mapSystem->GetMapElementList(), LevelGeneratedMapElement::GetType_static() ))
    {
        levelGenerated->PlugInNodeId( LevelGeneratedMapElement::GeneratedNodeId(), spawnActorMapElement->GetInputNodeId( SpawnActorMapElement::SpawnNodeId() ) );
    }
}

void SpawnProperty::RespawnActor( Opt<RespawnActorMapElement> respawnActorMapElement, glm::vec2 & pos )
{
    static int32_t componentId = AutoId( "position_component" );
    static Opt<MapSystem> mapSystem = MapSystem::Get();
    Opt<PositionComponentLoader> positionCompLoader = respawnActorMapElement->GetComponentLoader( componentId );
    if (positionCompLoader.IsValid())
    {
        positionCompLoader->Bind<double>( &PositionComponent::AddX, pos.x );
        positionCompLoader->Bind<double>( &PositionComponent::AddY, pos.y );
    }
    for (Opt<LevelGeneratedMapElement> levelGenerated : MapElementListFilter<MapSystem::All>( mapSystem->GetMapElementList(), LevelGeneratedMapElement::GetType_static() ))
    {
        levelGenerated->PlugInNodeId( LevelGeneratedMapElement::GeneratedNodeId(), respawnActorMapElement->GetInputNodeId( SpawnActorMapElement::SpawnNodeId() ) );
    }
}

} // namespace map
