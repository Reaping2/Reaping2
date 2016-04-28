#ifndef INCLUDED_MAP_LEVEL_GENERATOR_MAP_ELEMENT_H
#define INCLUDED_MAP_LEVEL_GENERATOR_MAP_ELEMENT_H

#include "core/map/map_element.h"
#include "platform/i_platform.h"
#include "../base_input.h"

namespace map {

class LevelGeneratorMapElement : public MapElement, public BaseInput
{
public:
    DEFINE_MAP_ELEMENT_BASE( LevelGeneratorMapElement )
    LevelGeneratorMapElement( int32_t Id );
    void Load( Json::Value& setters );
    void SetGeneratorId( int32_t generatorId );
    int32_t GetGeneratorId()const;
    static int32_t GeneratorNodeId();
private:
    int32_t mGeneratorId;
};

} // namespace map

#endif//INCLUDED_MAP_LEVEL_GENERATOR_MAP_ELEMENT_H

//command:  "classgenerator.exe" -g "map_element" -c "level_generator" -m "int32_t-generatorId"
