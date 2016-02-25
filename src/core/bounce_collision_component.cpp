#include "core/bounce_collision_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

BounceCollisionComponent::BounceCollisionComponent()
    : mSpeedLossPercent(0.0)
{
}

void BounceCollisionComponent::SetSpeedLossPercent(double speedLossPercent)
{
    mSpeedLossPercent=speedLossPercent;
}

double BounceCollisionComponent::GetSpeedLossPercent()const
{
    return mSpeedLossPercent;
}



void BounceCollisionComponentLoader::BindValues()
{
    Bind("speed_loss_percent",func_double(&BounceCollisionComponent::SetSpeedLossPercent));
}

BounceCollisionComponentLoader::BounceCollisionComponentLoader()
{
    SetBase<CollisionComponentLoader>();
}

BOOST_CLASS_EXPORT_IMPLEMENT(BounceCollisionComponent);
