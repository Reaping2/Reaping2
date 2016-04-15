#include "core/bounce_collision_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

BounceCollisionComponent::BounceCollisionComponent()
    : ShotCollisionComponent()
    , mSpeedLossPercent( 0.0 )
    , mUseShotCollision( false )
    , mResetActorsCollidedOnBounce(false)
{
}

void BounceCollisionComponent::SetSpeedLossPercent( double speedLossPercent )
{
    mSpeedLossPercent = speedLossPercent;
}

double BounceCollisionComponent::GetSpeedLossPercent()const
{
    return mSpeedLossPercent;
}

void BounceCollisionComponent::SetUseShotCollision(bool useShotCollision)
{
    mUseShotCollision = useShotCollision;
}

bool BounceCollisionComponent::IsUseShotCollision()const
{
    return mUseShotCollision;
}

void BounceCollisionComponent::SetResetActorsCollidedOnBounce(bool resetActorsCollidedOnBounce)
{
    mResetActorsCollidedOnBounce = resetActorsCollidedOnBounce;
}

bool BounceCollisionComponent::IsResetActorsCollidedOnBounce()const
{
    return mResetActorsCollidedOnBounce;
}

void BounceCollisionComponentLoader::BindValues()
{
    Bind( "speed_loss_percent", func_double( &BounceCollisionComponent::SetSpeedLossPercent ) );
    Bind( "use_shot", func_bool( &BounceCollisionComponent::SetUseShotCollision ) );
    Bind( "reset_actors_collided_on_bounce", func_bool(&BounceCollisionComponent::SetResetActorsCollidedOnBounce));
}

BounceCollisionComponentLoader::BounceCollisionComponentLoader()
{
    SetBase<ShotCollisionComponentLoader>();
}

REAPING2_CLASS_EXPORT_IMPLEMENT( BounceCollisionComponent, BounceCollisionComponent );
