#include "move_component.h"

MoveComponent::MoveComponent()
    : mHeading(0.0)
    , mSpeed(0.0)
    , mSpeedX(0.0)
    , mSpeedY(0.0)
{
}
void MoveComponent::UpdateProjections()
{
    const double spd = GetSpeed();
    const double h = GetHeading();
    const double c = cos( h );
    const double s = sin( h );
    SetSpeedX(c * spd);
    SetSpeedY(s * spd);
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
    UpdateProjections();
}
void MoveComponent::SetSpeed( double Speed )
{
    mSpeed = Speed;
    UpdateProjections();
}
void MoveComponent::SetSpeedX( double SpeedX )
{
    mSpeedX = SpeedX;
}
void MoveComponent::SetSpeedY( double SpeedY )
{
    mSpeedY = SpeedY;
}

void MoveComponentLoader::BindValues()
{
    Bind("speed",func_double(&MoveComponent::SetSpeed));
}

MoveComponentLoader::MoveComponentLoader()
{

}
