#include "core/light_component.h"

LightComponent::LightComponent()
    : mRadius(0.0)
{
}

void LightComponent::SetRadius( double radius )
{
    mRadius = radius;
}

double LightComponent::GetRadius() const
{
    return mRadius;
}



void LightComponentLoader::BindValues()
{
}

LightComponentLoader::LightComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( LightComponent, LightComponent );
