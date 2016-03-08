#include "core/map/soldier_spawn_point_map_element.h"

namespace map {

SoldierSpawnPointMapElement::SoldierSpawnPointMapElement( int32_t Id )
    : MapElement( Id )
    , mX( 0 )
    , mY( 0 )
{
}

void SoldierSpawnPointMapElement::Load( Json::Value& setters )
{
    MapElement::Load( setters );
    Json::Value const& position = setters["position"];
    if( !position.isObject() )
    {
        return;
    }
    int32_t x;
    if ( !Json::GetInt( position["x"], x ) )
    {
        return;
    }
    SetX( x );
    int32_t y;
    if ( !Json::GetInt( position["y"], y ) )
    {
        return;
    }
    SetY( y );
}

void SoldierSpawnPointMapElement::SetX( int32_t x )
{
    mX = x;
}

int32_t SoldierSpawnPointMapElement::GetX()const
{
    return mX;
}

void SoldierSpawnPointMapElement::SetY( int32_t y )
{
    mY = y;
}

int32_t SoldierSpawnPointMapElement::GetY()const
{
    return mY;
}


} // namespace map
