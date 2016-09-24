#include "platform/i_platform.h"
#include "level_generator_factory.h"
#include "jungle_level_generator.h"

using platform::AutoId;
namespace map {

LevelGeneratorFactory::LevelGeneratorFactory()
{
    Bind( AutoId("default_level_generator"), &CreateLevelGenerator<DefaultLevelGenerator> );
    SetDefault( AutoId("default_level_generator") );
    Bind( AutoId( "jungle" ), &CreateLevelGenerator<JungleLevelGenerator> );
}

} // namespace map

