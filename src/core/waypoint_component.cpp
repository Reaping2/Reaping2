#include "core/waypoint_component.h"

WaypointComponent::WaypointComponent()
{
}


void WaypointComponent::SetLit( bool lit )
{
    mLit = lit;
}

bool WaypointComponent::IsLit() const
{
    return mLit;
}


void WaypointComponentLoader::BindValues()
{
}

WaypointComponentLoader::WaypointComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( WaypointComponent, WaypointComponent );
