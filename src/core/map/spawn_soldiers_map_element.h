#ifndef INCLUDED_MAP_SPAWN_SOLDIERS_H
#define INCLUDED_MAP_SPAWN_SOLDIERS_H

#include "core/map/map_element.h"
#include "platform/i_platform.h"
#include "base_input.h"

namespace map {

class SpawnSoldiersMapElement : public MapElement, public BaseInput
{
public:
    DEFINE_MAP_ELEMENT_BASE(SpawnSoldiersMapElement)
    SpawnSoldiersMapElement();
    void Load(Json::Value& setters);
    static int32_t SpawnNodeId;
    int32_t mSpawnInputCounter;
private:
    void SpawnInput(int32_t in);
};

} // namespace map

#endif//INCLUDED_MAP_SPAWN_SOLDIERS_H

