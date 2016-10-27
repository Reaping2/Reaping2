#include "core/path_controller_component.h"

PathControllerComponent::PathControllerComponent()
    : mNextAttackTimer(0.0)
    , mNextAttackTimerMax(3.0)
    , mDamage(10)
    , mAggroDist(800)
    , mPeaceDist(1200)
{
}

void PathControllerComponent::SetNextAttackTimer( double nextAttackTimer )
{
    mNextAttackTimer = nextAttackTimer;
}

double PathControllerComponent::GetNextAttackTimer() const
{
    return mNextAttackTimer;
}

void PathControllerComponent::SetNextAttackTimerMax( double nextAttackTimerMax )
{
    mNextAttackTimerMax = nextAttackTimerMax;
}

double PathControllerComponent::GetNextAttackTimerMax() const
{
    return mNextAttackTimerMax;
}

void PathControllerComponent::SetDamage( int32_t damage )
{
    mDamage = damage;
}

int32_t PathControllerComponent::GetDamage() const
{
    return mDamage;
}

void PathControllerComponent::SetAggroDist( int32_t aggroDist )
{
    mAggroDist = aggroDist;
}

int32_t PathControllerComponent::GetAggroDist() const
{
    return mAggroDist;
}

void PathControllerComponent::SetPeaceDist( int32_t peaceDist )
{
    mPeaceDist = peaceDist;
}

int32_t PathControllerComponent::GetPeaceDist() const
{
    return mPeaceDist;
}



void PathControllerComponentLoader::BindValues()
{
    Bind( "aggro_distance", func_int32_t( &PathControllerComponent::SetAggroDist ) );
    Bind( "next_attack_timer", func_int32_t( &PathControllerComponent::SetNextAttackTimerMax ) );
    Bind( "damage", func_int32_t( &PathControllerComponent::SetDamage ) );
    Bind( "peace_distance", func_int32_t( &PathControllerComponent::SetPeaceDist ) );
}

PathControllerComponentLoader::PathControllerComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( PathControllerComponent, PathControllerComponent );
