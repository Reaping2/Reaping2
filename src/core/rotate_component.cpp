#include "core/rotate_component.h"

RotateComponent::RotateComponent()
    : mSpeed(0.0)
    , mRotating(true)
{
}

void RotateComponent::SetSpeed(double speed)
{
    mSpeed=speed;
}

double RotateComponent::GetSpeed()const
{
    return mSpeed;
}

void RotateComponent::SetRotating(bool rotating)
{
    mRotating=rotating;
}

bool RotateComponent::IsRotating()const
{
    return mRotating;
}



void RotateComponentLoader::BindValues()
{
    Bind( "speed", func_double( &RotateComponent::SetSpeed ) );
    Bind( "rotating", func_bool( &RotateComponent::SetRotating ) );
}

RotateComponentLoader::RotateComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( RotateComponent, RotateComponent );
