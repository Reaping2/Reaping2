#include "level_generator_map_element.h"

namespace map {

LevelGeneratorMapElement::LevelGeneratorMapElement( int32_t Id )
    : MapElement( Id )
    , BaseInput()
    , mGeneratorId( -1 )
{
}

void LevelGeneratorMapElement::Load( Json::Value const& setters )
{
    MapElement::Load( setters );
    std::string identifier;
    if (Json::GetStr( setters["generator_name"], identifier ))
    {
        mGeneratorId = AutoId( identifier );
    }
    static LevelGeneratorFactory& levelGeneratorFactory = LevelGeneratorFactory::Get();
    auto generator = levelGeneratorFactory( mGeneratorId );
    generator->Load( setters["properties"] );
    mLevelGenerator.reset( generator.release() );
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
    static int32_t id = AutoId( "generate" );
    return id;
}


Opt<ILevelGenerator> LevelGeneratorMapElement::GetLevelGenerator()
{
    return mLevelGenerator.get();
}

} // namespace map
