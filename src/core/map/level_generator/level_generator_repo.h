#ifndef INCLUDED_MAP_LEVEL_GENERATOR_REPO_H
#define INCLUDED_MAP_LEVEL_GENERATOR_REPO_H

#include "platform/repository.h"
#include "platform/singleton.h"
#include "i_level_generator.h"

namespace map {

    class DefaultLevelGenerator : public ILevelGenerator
    {
    public:
        DefaultLevelGenerator();
        virtual void Generate();
    };

class LevelGeneratorRepo : public platform::Repository<ILevelGenerator>, public platform::Singleton<LevelGeneratorRepo>
{
    friend class platform::Singleton<LevelGeneratorRepo>;
    static DefaultLevelGenerator const mDefault;
    LevelGeneratorRepo();
};

} // namespace map

#endif//INCLUDED_MAP_LEVEL_GENERATOR_REPO_H

//command:  "classgenerator.exe" -g "repository" -c "level_generator_repo" -t "i_level_generator" -n "map"
