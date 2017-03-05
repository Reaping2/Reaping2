#ifndef INCLUDED_MAP_LEVEL_GENERATED_H
#define INCLUDED_MAP_LEVEL_GENERATED_H

#include "core/map/map_element.h"
#include "platform/i_platform.h"
#include "../base_output.h"

namespace map {

class LevelGeneratedMapElement : public MapElement, public BaseOutput
{
public:
    DEFINE_MAP_ELEMENT_BASE(LevelGeneratedMapElement)
    LevelGeneratedMapElement(int32_t Id);
    void Load(Json::Value const& setters);
    static int32_t GeneratedNodeId();
    static int32_t ActorsSpawnedNodeId();
private:
};

} // namespace map

#endif//INCLUDED_MAP_LEVEL_GENERATED_H

//command:  "classgenerator.exe" -g "map_element" -c "level_generated"
