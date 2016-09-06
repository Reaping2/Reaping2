#ifndef INCLUDED_MAP_LEVEL_GENERATOR_FACTORY_H
#define INCLUDED_MAP_LEVEL_GENERATOR_FACTORY_H

#include "platform/factory.h"
#include "platform/singleton.h"
#include "i_level_generator.h"

namespace map {

class LevelGeneratorFactory : public platform::Factory<ILevelGenerator>, public platform::Singleton<LevelGeneratorFactory>
{
    friend class platform::Singleton<LevelGeneratorFactory>;
    LevelGeneratorFactory();
protected:
    template<typename Elem_T>
    static std::auto_ptr<ILevelGenerator> CreateLevelGenerator( int32_t Id );
};

template<typename Elem_T>
std::auto_ptr<ILevelGenerator> LevelGeneratorFactory::CreateLevelGenerator( int32_t Id )
{
    return std::auto_ptr<ILevelGenerator>( new Elem_T( Id ) );
}

} // namespace map

#endif//INCLUDED_MAP_LEVEL_GENERATOR_FACTORY_H

//command:  "classgenerator.exe" -g "factory" -c "level_generator_factory" -n "map" -t "level_generator"
