#ifndef INCLUDED_MAP_GROUP_MAP_ELEMENT_SYSTEM_H
#define INCLUDED_MAP_GROUP_MAP_ELEMENT_SYSTEM_H

#include "core/map/map_element_system.h"

namespace map {

class GroupMapElementSystem : public MapElementSystem
{
public:
    DEFINE_SYSTEM_BASE(GroupMapElementSystem)
    GroupMapElementSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
};

} // namespace map

#endif//INCLUDED_MAP_GROUP_MAP_ELEMENT_SYSTEM_H

//command:  "classgenerator.exe" -g "map_element_system" -c "group_map_element_system" -t "group_map_element"
