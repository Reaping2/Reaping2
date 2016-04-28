#include "platform/i_platform.h"
#include "level_generator_repo.h"
#include "jungle_level_generator.h"

using platform::AutoId;

namespace map {

DefaultLevelGenerator const LevelGeneratorRepo::mDefault = DefaultLevelGenerator();

LevelGeneratorRepo::LevelGeneratorRepo()
    : Repository<ILevelGenerator>( mDefault )
{
    int32_t id = AutoId( "jungle" );
    mElements.insert( id, new JungleLevelGenerator( id ) );
}

DefaultLevelGenerator::DefaultLevelGenerator()
    : ILevelGenerator( -1 )
{

}

void DefaultLevelGenerator::Generate()
{

}

} // namespace map

