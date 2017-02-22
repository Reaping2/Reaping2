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

void LightComponent::SetAperture( double radius )
{
    mAperture = radius;
}

double LightComponent::GetAperture() const
{
    return mAperture;
}

void LightComponentLoader::BindValues()
{
    Bind( "radius", func_double( &LightComponent::SetRadius ) );
    Bind( "aperture", func_double( &LightComponent::SetAperture ) );
}

LightComponentLoader::LightComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( LightComponent, LightComponent );
