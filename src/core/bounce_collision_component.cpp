#include "core/bounce_collision_component.h"

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
