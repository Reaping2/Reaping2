#ifndef INCLUDED_MAP_I_LEVEL_GENERATOR_H
#define INCLUDED_MAP_I_LEVEL_GENERATOR_H

#include "platform/i_platform.h"
#include "../../scene.h"
#include "../../actor_factory.h"

namespace map {

class ILevelGenerator
{
public:
    ILevelGenerator( int32_t Id );
    virtual ~ILevelGenerator();
    virtual void Generate() = 0;
protected:
    int32_t mId = -1;
    Scene& mScene;
};

} // namespace map

#endif//INCLUDED_MAP_I_LEVEL_GENERATOR_H


//command:  "classgenerator.exe" -g "enum" -c "level_generator" -m "int32_t-Id" -n "map"
