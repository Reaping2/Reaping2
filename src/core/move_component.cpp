#include "move_component.h"

MoveComponent::MoveComponent()
    : mHeading(0.0)
    , mSpeed(0.0)
    , mSpeedX(0.0)
    , mSpeedY(0.0)
{
}

double const& MoveComponent::GetHeading() const
{
    return mHeading;
}
double const& MoveComponent::GetSpeed() const
{
    return mSpeed;
}
double const& MoveComponent::GetSpeedX() const
{
    return mSpeedX;
}
double const& MoveComponent::GetSpeedY() const
{
    return mSpeedY;
}
void MoveComponent::SetHeading( double Heading )
{
    mHeading = Heading;
}
void MoveComponent::SetSpeed( double Speed )
{
    mSpeed = Speed;
}
void MoveComponent::SetSpeedX( double SpeedX )
{
    mSpeedX = SpeedX;
}
void MoveComponent::SetSpeedY( double SpeedY )
{
    mSpeedY = SpeedY;
}
