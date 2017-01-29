#include "core/worm_body_component.h"

WormBodyComponent::WormBodyComponent()
    : mHeadGUID(-1)
    , mBody(true)
    , mFollowedGUID(-1)
    , mPrevPositions()
    , mWaitFullDistance(true)
    , mDistancePercent(1.0)
{
}

void WormBodyComponent::SetHeadGUID( int32_t headGUID )
{
    mHeadGUID = headGUID;
}

int32_t WormBodyComponent::GetHeadGUID() const
{
    return mHeadGUID;
}

void WormBodyComponent::SetBody( bool body )
{
    mBody = body;
}

bool WormBodyComponent::IsBody() const
{
    return mBody;
}

void WormBodyComponent::SetFollowedGUID( int32_t followedGUID )
{
    mFollowedGUID = followedGUID;
}

int32_t WormBodyComponent::GetFollowedGUID() const
{
    return mFollowedGUID;
}

void WormBodyComponent::SetPrevPositions( PrevPositions_t const& prevPositions )
{
    mPrevPositions = prevPositions;
}

IWormBodyComponent::PrevPositions_t& WormBodyComponent::GetPrevPositions()
{
    return mPrevPositions;
}

void WormBodyComponent::SetWaitFullDistance( bool waitFullDistance )
{
    mWaitFullDistance = waitFullDistance;
}

bool WormBodyComponent::IsWaitFullDistance() const
{
    return mWaitFullDistance;
}

void WormBodyComponent::SetDistancePercent( double distancePercent )
{
    mDistancePercent = distancePercent;
}

double WormBodyComponent::GetDistancePercent() const
{
    return mDistancePercent;
}

void WormBodyComponentLoader::BindValues()
{
    Bind( "body", func_bool( &WormBodyComponent::SetBody ) );
    Bind( "distance_percent", func_double( &WormBodyComponent::SetDistancePercent ) );
}

WormBodyComponentLoader::WormBodyComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( WormBodyComponent, WormBodyComponent );
