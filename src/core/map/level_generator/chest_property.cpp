#include "chest_property.h"
#include "spawn_property.h"
#include "core/pickup_profiles_repo.h"
#include "core/position_component.h"
#include "level_generated_map_element.h"


namespace map {

ChestProperty::ChestProperty( int32_t Id )
    : IProperty( Id )
    , mTargets()
{
}

void ChestProperty::Load( Json::Value const& setters )
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
    Json::GetDouble( setters["x"], mChestX );
    Json::GetDouble( setters["y"], mChestY );
}


void ChestProperty::Save( Json::Value& setters ) const
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
    setters["x"] = mChestX;
    setters["y"] = mChestY;
}

void ChestProperty::SetTargets( Targets_t blockedTargets )
{
    mTargets = blockedTargets;
}

ChestProperty::Targets_t const& ChestProperty::GetTargets() const
{
    return mTargets;
}



void ChestProperty::Generate( RoomDesc& roomDesc, MapElementHolder& mMapElementHolder, glm::vec2 pos, bool editor /*= false*/ )
{
    if (roomDesc.HasProperty( RoomProperty::Chest ) || editor)
    {
        SpawnProperty::SpawnTargets( roomDesc, mTargets, mMapElementHolder, pos );
    }
    if (roomDesc.GetChestId() != -1)
    {
        std::auto_ptr<MapElement> mapElement( MapElementFactory::Get()( AutoId( "spawn_actor" ) ) );
        Opt<SpawnActorMapElement> spawnActor( static_cast<SpawnActorMapElement*>( mapElement.get() ) );
        spawnActor->SetActorID( roomDesc.GetChestId() );

        spawnActor->AddComponentLoader( AutoId( "position_component" ), 
            std::move(PositionComponentLoader::FromPosition( mChestX + pos.x , mChestY + pos.y )) );

        static Opt<MapSystem> mMapSystem = MapSystem::Get();

        for (Opt<LevelGeneratedMapElement> levelGenerated : MapElementListFilter<MapSystem::All>( mMapSystem->GetMapElementList(), LevelGeneratedMapElement::GetType_static() ))
        {
            levelGenerated->PlugInNodeId( LevelGeneratedMapElement::GeneratedNodeId(), spawnActor->GetInputNodeId( SpawnActorMapElement::SpawnNodeId() ) );
        }
        mMapSystem->GetMapElementList().insert( mapElement.release() );
    }
}

} // namespace map
