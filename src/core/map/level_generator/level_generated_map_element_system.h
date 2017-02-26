#ifndef INCLUDED_MAP_LEVEL_GENERATED_MAP_ELEMENT_SYSTEM_H
#define INCLUDED_MAP_LEVEL_GENERATED_MAP_ELEMENT_SYSTEM_H

#include "core/map/map_element_system.h"
#include "level_generated_event.h"

namespace map {

class LevelGeneratedMapElementSystem : public MapElementSystem
{
public:
    DEFINE_SYSTEM_BASE(LevelGeneratedMapElementSystem)
    LevelGeneratedMapElementSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    AutoReg mOnLevelGenerated;
    void OnLevelGenerated(map::LevelGeneratedEvent const& Evt);
    int32_t mHandleTerrainGeneratedCounter = -1;
};

} // namespace map

#endif//INCLUDED_MAP_LEVEL_GENERATED_MAP_ELEMENT_SYSTEM_H

//command:  "classgenerator.exe" -g "map_element_system" -c "level_generated_map_element_system" -t "level_generated_map_element" -e "map-levelGenerated"
