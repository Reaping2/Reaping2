#ifndef INCLUDED_MAP_SOLDIER_AUTO_REVIVE_H
#define INCLUDED_MAP_SOLDIER_AUTO_REVIVE_H

#include "core/map/map_element.h"
#include "platform/i_platform.h"

namespace map {

class SoldierAutoReviveMapElement : public MapElement
{
public:
    DEFINE_MAP_ELEMENT_BASE( SoldierAutoReviveMapElement )
    SoldierAutoReviveMapElement( int32_t Id );
    void Load( Json::Value const& setters );
    void SetSecsToRevive( double secsToRevive );
    double GetSecsToRevive()const;
private:
    double mSecsToRevive;
};

} // namespace map

#endif//INCLUDED_MAP_SOLDIER_AUTO_REVIVE_H

//command:  "classgenerator.exe" -g "map_element" -c "soldier_auto_revive" -m "double-secsToRevive"
