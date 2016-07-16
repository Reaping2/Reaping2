#include "core/map/level_generator/level_generated_map_element.h"

namespace map {

LevelGeneratedMapElement::LevelGeneratedMapElement(int32_t Id)
    : MapElement(Id)
    , BaseOutput() 
{
    AddOutputNodeId( GeneratedNodeId() );
}

void LevelGeneratedMapElement::Load(Json::Value& setters)
{
    MapElement::Load(setters);
}


int32_t LevelGeneratedMapElement::GeneratedNodeId()
{
    static int32_t id = AutoId( "generated" );
    return id;
}

} // namespace map
