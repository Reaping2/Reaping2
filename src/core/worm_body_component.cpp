#include "core/worm_body_component.h"

WormBodyComponent::WormBodyComponent()
    : mHeadGUID(-1)
    , mBody(true)
    , mFollowedGUID(-1)
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

void WormBodyComponentLoader::BindValues()
{
    Bind( "body", func_bool( &WormBodyComponent::SetBody ) );
}

WormBodyComponentLoader::WormBodyComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( WormBodyComponent, WormBodyComponent );
