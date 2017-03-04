#ifndef INCLUDED_MAP_SOLDIER_SPAWN_POINT_H
#define INCLUDED_MAP_SOLDIER_SPAWN_POINT_H

#include "core/map/map_element.h"
#include "platform/i_platform.h"

namespace map {

class SoldierSpawnPointMapElement : public MapElement
{
public:
    DEFINE_MAP_ELEMENT_BASE( SoldierSpawnPointMapElement )
    SoldierSpawnPointMapElement( int32_t Id );
    virtual void Load( Json::Value const& setters );
    virtual void Save( Json::Value& Element ); 
    void SetX( int32_t x );
    int32_t GetX()const;
    void SetY( int32_t y );
    int32_t GetY()const;
private:
    int32_t mX;
    int32_t mY;
};

} // namespace map

#endif//INCLUDED_MAP_SOLDIER_SPAWN_POINT_H
