#include "platform/i_platform.h"
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
    mX = std::floor(X*PRECISION)/PRECISION;
}
void PositionComponent::SetY( double Y )
{
    mY = std::floor(Y*PRECISION)/PRECISION;
}
void PositionComponent::SetOrientation( double Orientation )
{
    mOrientation = std::floor(Orientation*PRECISION)/PRECISION;
}

void PositionComponentLoader::BindValues()
{

}

PositionComponentLoader::PositionComponentLoader()
{

}
