#ifndef INCLUDED_MAP_MAP_START_H
#define INCLUDED_MAP_MAP_START_H

#include "core/map/map_element.h"
#include "platform/i_platform.h"
#include "base_output.h"
#include "base_input.h"

namespace map {

class MapStartMapElement : public MapElement, public BaseOutput, public BaseInput
{
public:
    DEFINE_MAP_ELEMENT_BASE( MapStartMapElement )
    MapStartMapElement( int32_t Id );
    void Load( Json::Value const& setters );
    static int32_t StartInputNodeId();
    static int32_t StartOutputNodeId();
    static int32_t PreActorsSpawnedInputNodeId();
    static int32_t PreActorsSpawnedOutputNodeId();
    static int32_t ActorsSpawnedInputNodeId();
    static int32_t ActorsSpawnedOutputNodeId();
    static int32_t ReadyInputNodeId();
    static int32_t ReadyOutputNodeId();
private:
};

} // namespace map

#endif//INCLUDED_MAP_MAP_START_H

