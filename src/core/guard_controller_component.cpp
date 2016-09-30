#include "core/guard_controller_component.h"

GuardControllerComponent::GuardControllerComponent()
    : mNextMoveTimer( 0.0 )
    , mMoveDirection( None )
    , mAggroAltDist( 600 )
    , mCloseDist( 300 )
    , mWalkAwayDist( 200 )
    , mPeaceDist( 1100 )
    , mNextMoveTimerMax( 2.0 )
    , mNextMoveTimerVariance( 2.0 )
    , mAggroDist( 750 )
{
}

void GuardControllerComponent::SetNextMoveTimer( double nextMoveTimer )
{
    mNextMoveTimer = nextMoveTimer;
}

double GuardControllerComponent::GetNextMoveTimer() const
{
    return mNextMoveTimer;
}

void GuardControllerComponent::SetMoveDirection( Direction moveDirection )
{
    mMoveDirection = moveDirection;
}

GuardControllerComponent::Direction GuardControllerComponent::GetMoveDirection() const
{
    return mMoveDirection;
}

void GuardControllerComponent::SetAggroAltDist( int32_t aggroAltDist )
{
    mAggroAltDist = aggroAltDist;
}

int32_t GuardControllerComponent::GetAggroAltDist() const
{
    return mAggroAltDist;
}

void GuardControllerComponent::SetCloseDist( int32_t closeDist )
{
    mCloseDist = closeDist;
}

int32_t GuardControllerComponent::GetCloseDist() const
{
    return mCloseDist;
}

void GuardControllerComponent::SetWalkAwayDist( int32_t walkAwayDist )
{
    mWalkAwayDist = walkAwayDist;
}

int32_t GuardControllerComponent::GetWalkAwayDist() const
{
    return mWalkAwayDist;
}

void GuardControllerComponent::SetPeaceDist( int32_t peaceDist )
{
    mPeaceDist = peaceDist;
}

int32_t GuardControllerComponent::GetPeaceDist() const
{
    return mPeaceDist;
}

void GuardControllerComponent::SetNextMoveTimerMax( double nextMoveTimerMax )
{
    mNextMoveTimerMax = nextMoveTimerMax;
}

double GuardControllerComponent::GetNextMoveTimerMax() const
{
    return mNextMoveTimerMax;
}

void GuardControllerComponent::SetNextMoveTimerVariance( double nextMoveTimerVariance )
{
    mNextMoveTimerVariance = nextMoveTimerVariance;
}

double GuardControllerComponent::GetNextMoveTimerVariance() const
{
    return mNextMoveTimerVariance;
}

void GuardControllerComponent::SetAggroDist( int32_t aggroDist )
{
    mAggroDist = aggroDist;
}

int32_t GuardControllerComponent::GetAggroDist() const
{
    return mAggroDist;
}



void GuardControllerComponentLoader::BindValues()
{
    Bind( "aggro_distance", func_int32_t( &GuardControllerComponent::SetAggroDist ) );
    Bind( "aggro_alt_distance", func_int32_t(&GuardControllerComponent::SetAggroAltDist) );
    Bind( "close_distance", func_int32_t( &GuardControllerComponent::SetCloseDist ) );
    Bind( "walk_away_distance", func_int32_t( &GuardControllerComponent::SetWalkAwayDist ) );
    Bind( "peace_distance", func_int32_t( &GuardControllerComponent::SetPeaceDist ) );
    Bind( "next_move_timer", func_double( &GuardControllerComponent::SetNextMoveTimerMax ) );
    Bind( "next_move_timer_variance", func_double( &GuardControllerComponent::SetNextMoveTimerVariance ) );
}

GuardControllerComponentLoader::GuardControllerComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( GuardControllerComponent, GuardControllerComponent );
