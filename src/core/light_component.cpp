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

void LightComponent::SetAperture( double aperture )
{
    mAperture = aperture;
}

double LightComponent::GetAperture() const
{
    return mAperture;
}

void LightComponent::SetFullStrengthAperture( double aperture )
{
    mFSAperture = aperture;
}

double LightComponent::GetFullStrengthAperture() const
{
    return std::min( mFSAperture, mAperture );
}

void LightComponentLoader::BindValues()
{
    Bind( "radius", func_double( &LightComponent::SetRadius ) );
    Bind( "aperture", func_double( &LightComponent::SetAperture ) );
    Bind( "full_strength_aperture", func_double( &LightComponent::SetFullStrengthAperture ) );
}

LightComponentLoader::LightComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( LightComponent, LightComponent );
