#include "core/explosion_component.h"

ExplosionComponent::ExplosionComponent()
    : mStartRadius(0.0)
    , mMaxRadius(0.0)
    , mScaleSpeed(0.0)
{
}

void ExplosionComponent::SetStartRadius(double startRadius)
{
    mStartRadius=startRadius;
}

double ExplosionComponent::GetStartRadius()const
{
    return mStartRadius;
}

void ExplosionComponent::SetMaxRadius(double maxRadius)
{
    mMaxRadius=maxRadius;
}

double ExplosionComponent::GetMaxRadius()const
{
    return mMaxRadius;
}

void ExplosionComponent::SetScaleSpeed(double scaleSpeed)
{
    mScaleSpeed=scaleSpeed;
}

double ExplosionComponent::GetScaleSpeed()const
{
    return mScaleSpeed;
}



void ExplosionComponentLoader::BindValues()
{
    Bind("start_radius",func_double(&ExplosionComponent::SetStartRadius));
    Bind("max_radius",func_double(&ExplosionComponent::SetMaxRadius));
    Bind("scale_speed",func_double(&ExplosionComponent::SetScaleSpeed));
}

ExplosionComponentLoader::ExplosionComponentLoader()
{
}
