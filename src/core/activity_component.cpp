#include "core/activity_component.h"

ActivityComponent::ActivityComponent()
    : mRadius(0.0)
{
}

void ActivityComponent::SetRadius( double radius )
{
    mRadius = radius;
}

double ActivityComponent::GetRadius() const
{
    return mRadius;
}



void ActivityComponentLoader::BindValues()
{
    Bind( "radius", func_double( &ActivityComponent::SetRadius ) );
}

ActivityComponentLoader::ActivityComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( ActivityComponent, ActivityComponent );
