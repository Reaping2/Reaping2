#include "core/map/soldier_auto_revive_map_element.h"

namespace map {

SoldierAutoReviveMapElement::SoldierAutoReviveMapElement( int32_t Id )
    : MapElement( Id )
    , mSecsToRevive( 0.0 )
{
}

void SoldierAutoReviveMapElement::Load( Json::Value const& setters )
{
    MapElement::Load( setters );
    double secsToRevive;
    if ( !Json::GetDouble( setters["secs_to_revive"], secsToRevive ) )
    {
        return;
    }
    SetSecsToRevive( secsToRevive );
}

void SoldierAutoReviveMapElement::SetSecsToRevive( double secsToRevive )
{
    mSecsToRevive = secsToRevive;
}

double SoldierAutoReviveMapElement::GetSecsToRevive()const
{
    return mSecsToRevive;
}


} // namespace map
