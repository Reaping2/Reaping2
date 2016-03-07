#ifndef INCLUDED_MAP_SOLDIER_AUTO_REVIVE_MAP_ELEMENT_SYSTEM_H
#define INCLUDED_MAP_SOLDIER_AUTO_REVIVE_MAP_ELEMENT_SYSTEM_H

#include "core/map/map_element_system.h"

namespace map {

class SoldierAutoReviveMapElementSystem : public MapElementSystem
{
public:
    DEFINE_SYSTEM_BASE( SoldierAutoReviveMapElementSystem )
    SoldierAutoReviveMapElementSystem();
    static Opt<SoldierAutoReviveMapElementSystem> Get();
    double GetSecsToRevive();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
};

} // namespace map

#endif//INCLUDED_MAP_SOLDIER_AUTO_REVIVE_MAP_ELEMENT_SYSTEM_H

//command:  "classgenerator.exe" -g "map_element_system" -c "soldier_auto_revive_map_element_system" -t "soldier_auto_revive_map_element"
