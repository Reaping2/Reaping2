#ifndef INCLUDED_MAP_LEVEL_GENERATOR_MAP_ELEMENT_SYSTEM_H
#define INCLUDED_MAP_LEVEL_GENERATOR_MAP_ELEMENT_SYSTEM_H

#include "core/map/map_element_system.h"

namespace map {

class LevelGeneratorMapElementSystem : public MapElementSystem
{
public:
    DEFINE_SYSTEM_BASE(LevelGeneratorMapElementSystem)
    LevelGeneratorMapElementSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
};

} // namespace map

#endif//INCLUDED_MAP_LEVEL_GENERATOR_MAP_ELEMENT_SYSTEM_H


//command:  "classgenerator.exe" -g "map_element_system" -c "level_generator_map_element_system" -t "level_generator_map_element"
