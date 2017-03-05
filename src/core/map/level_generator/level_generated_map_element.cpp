#include "core/map/level_generator/level_generated_map_element.h"

namespace map {

LevelGeneratedMapElement::LevelGeneratedMapElement(int32_t Id)
    : MapElement(Id)
    , BaseOutput() 
{
    AddOutputNodeId( GeneratedNodeId() );
    AddOutputNodeId( ActorsSpawnedNodeId() );
}

void LevelGeneratedMapElement::Load(Json::Value const& setters)
{
    MapElement::Load(setters);
}


int32_t LevelGeneratedMapElement::GeneratedNodeId()
{
    static int32_t id = AutoId( "generated" );
    return id;
}


int32_t LevelGeneratedMapElement::ActorsSpawnedNodeId()
{
    static int32_t id = AutoId( "actors_spawned" );
    return id;
}

} // namespace map
