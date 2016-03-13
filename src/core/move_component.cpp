#include "move_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

MoveComponent::MoveComponent()
    : mHeading( 0.0 )
    , mHeadingCos( cos( mHeading ) )
    , mHeadingSin( sin( mHeading ) )
    , mHeadingModifier( 0.0 )
    , mSpeedX( 0.0 )
    , mSpeedY( 0.0 )
    , mMoving( true )
    , mRooted( false )
{
    mSpeed.mBase.Init( 350.0, 0.0, 100000.0 );
    mSpeed.mFlat.Init( 0.0, 0.0, 33.0 );
    mSpeed.mPercent.Init( 0.0, 0.0, 30.0 );
}

double const& MoveComponent::GetHeading() const
{
    return mHeading;
}

double MoveComponent::GetSpeedX() const
{
    return mHeadingCos * mSpeed.Get(); //TODO: temporary for testing only
}
double MoveComponent::GetSpeedY() const
{
    return mHeadingSin * mSpeed.Get(); //TODO: temporary for testing only
}
void MoveComponent::SetHeading( double Heading )
{
    mHeading = std::floor( Heading * PRECISION ) / PRECISION;
    mHeadingCos = glm::round( cos( mHeading ) * PRECISION / 10 ) / PRECISION * 10;
    mHeadingSin = glm::round( sin( mHeading ) * PRECISION / 10 ) / PRECISION * 10;
}

double const& MoveComponent::GetHeadingModifier() const
{
    return mHeadingModifier;
}

void MoveComponent::SetHeadingModifier( double HeadingModifier )
{
    mHeadingModifier = std::floor( HeadingModifier * PRECISION ) / PRECISION;
}

Buffable<double>& MoveComponent::GetSpeed()
{
    return mSpeed;
}

bool MoveComponent::IsMoving() const
{
    return mMoving && !mRooted;
}

void MoveComponent::SetMoving( bool moving )
{
    mMoving = moving;
}

void MoveComponent::SetSpeed( double Speed )
{
    mSpeed.mBase.Set( Speed );
}

void MoveComponent::SetRooted( bool rooted )
{
    mRooted = rooted;
}

bool MoveComponent::IsRooted()const
{
    return mRooted;
}

bool MoveComponent::GetMoving() const
{
    return mMoving;
}


void MoveComponentLoader::BindValues()
{
    Bind( "speed", func_double( &MoveComponent::SetSpeed ) );
}

MoveComponentLoader::MoveComponentLoader()
{

}

REAPING2_CLASS_EXPORT_IMPLEMENT( MoveComponent, MoveComponent );
