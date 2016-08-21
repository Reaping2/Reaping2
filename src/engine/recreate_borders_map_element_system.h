#ifndef INCLUDED_MAP_RECREATE_BORDERS_MAP_ELEMENT_SYSTEM_H
#define INCLUDED_MAP_RECREATE_BORDERS_MAP_ELEMENT_SYSTEM_H

#include "core/map/map_element_system.h"
#include "core/map/neighbors.h"

namespace map {

class RecreateBordersMapElementSystem : public MapElementSystem
{
public:
    DEFINE_SYSTEM_BASE(RecreateBordersMapElementSystem)
    RecreateBordersMapElementSystem();
    Neighbors GetNeighbors( Actor& actor );
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    BorderType& mBorderType;
};

} // namespace map

#endif//INCLUDED_MAP_RECREATE_BORDERS_MAP_ELEMENT_SYSTEM_H

//command:  "classgenerator.exe" -g "map_element_system" -c "recreate_borders_map_element_system" -t "recreate_borders_map_element"
