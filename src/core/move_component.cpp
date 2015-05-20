#include "move_component.h"

MoveComponent::MoveComponent()
    : mHeading(0.0)
    , mHeadingCos(cos( mHeading ))
    , mHeadingSin(sin( mHeading ))
    , mHeadingModifier(0.0)
    , mSpeedX(0.0)
    , mSpeedY(0.0)
    , mMoving(true)
{
    mSpeed.mBase.Init(350.0,0.0,10000.0);
    mSpeed.mFlat.Init(0.0,0.0,1000.0);
    mSpeed.mPercent.Init(0.0,0.0,1000.0);
}

double const& MoveComponent::GetHeading() const
{
    return mHeading;
}

double MoveComponent::GetSpeedX() const
{
    return mHeadingCos*mSpeed.Get();
}
double MoveComponent::GetSpeedY() const
{
    return mHeadingSin*mSpeed.Get();
}
void MoveComponent::SetHeading( double Heading )
{
    mHeading = std::floor(Heading*PRECISION)/PRECISION;
    mHeadingCos = cos( mHeading );
    mHeadingSin = sin( mHeading );
}

double const& MoveComponent::GetHeadingModifier() const
{
    return mHeadingModifier;
}

void MoveComponent::SetHeadingModifier(double HeadingModifier)
{
    mHeadingModifier=std::floor(HeadingModifier*PRECISION)/PRECISION;
}

Buffable<double>& MoveComponent::GetSpeed()
{
    return mSpeed;
}

bool const& MoveComponent::IsMoving() const
{
    return mMoving;
}

void MoveComponent::SetMoving(bool moving)
{
    mMoving=moving;
}

void MoveComponent::SetSpeed(double Speed)
{
    mSpeed.mBase.Set(Speed);
}

void MoveComponentLoader::BindValues()
{
    Bind("speed",func_double(&MoveComponent::SetSpeed));
}

MoveComponentLoader::MoveComponentLoader()
{

}
