#include "move_component.h"

MoveComponent::MoveComponent()
    : mHeading(0.0)
    , mHeadingModifier(0.0)
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
    mHeading = std::floor(Heading*PRECISION)/PRECISION;
    UpdateProjections();
}
void MoveComponent::SetSpeed( double Speed )
{
    mSpeed = Speed;//std::floor(Speed*PRECISION)/PRECISION;
    UpdateProjections();
}
void MoveComponent::SetSpeedX( double SpeedX )
{
    mSpeedX = std::floor(SpeedX*PRECISION)/PRECISION;
}
void MoveComponent::SetSpeedY( double SpeedY )
{
    mSpeedY = std::floor(SpeedY*PRECISION)/PRECISION;
}

double const& MoveComponent::GetHeadingModifier() const
{
    return mHeadingModifier;
}

void MoveComponent::SetHeadingModifier(double HeadingModifier)
{
    mHeadingModifier=std::floor(HeadingModifier*PRECISION)/PRECISION;
}

void MoveComponentLoader::BindValues()
{
    Bind("speed",func_double(&MoveComponent::SetSpeed));
}

MoveComponentLoader::MoveComponentLoader()
{

}
