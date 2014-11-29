#include "i_core.h"
#include "core/position_component.h"


PositionComponent::PositionComponent()
    : mX(0.0)
    , mY(0.0)
    , mOrientation(0.0)
{
}

double const& PositionComponent::GetX()const
{
    return mX;
}
double const& PositionComponent::GetY()const
{
    return mY;
}
double const& PositionComponent::GetOrientation()const
{
    return mOrientation;
}
void PositionComponent::SetX( double X )
{
    mX = X;
}
void PositionComponent::SetY( double Y )
{
    mY = Y;
}
void PositionComponent::SetOrientation( double Orientation )
{
    mOrientation = Orientation;
}