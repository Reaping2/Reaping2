#include "level_generator_map_element.h"

namespace map {

LevelGeneratorMapElement::LevelGeneratorMapElement( int32_t Id )
    : MapElement( Id )
    , BaseInput()
    , mGeneratorId( -1 )
{
}

void LevelGeneratorMapElement::Load( Json::Value& setters )
{
    MapElement::Load( setters );
    std::string identifier;
    if (Json::GetStr( setters["generator_name"], identifier ))
    {
        mGeneratorId = AutoId( identifier );
    }
    AddInputNodeId( GeneratorNodeId() );
}

void LevelGeneratorMapElement::SetGeneratorId( int32_t generatorId )
{
    mGeneratorId = generatorId;
}

int32_t LevelGeneratorMapElement::GetGeneratorId()const
{
    return mGeneratorId;
}


int32_t LevelGeneratorMapElement::GeneratorNodeId()
{
    static int32_t id = AutoId( "generator" );
    return id;
}

} // namespace map
