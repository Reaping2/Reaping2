#include "core/activatable_component.h"

ActivatableComponent::ActivatableComponent()
    : mEnabled( true )
    , mActivatorGUID( -1 )
    , mHighlighted( false )
{
}

void ActivatableComponent::SetEnabled( bool enabled )
{
    mEnabled = enabled;
}

bool ActivatableComponent::IsEnabled() const
{
    return mEnabled;
}

void ActivatableComponent::SetActivatorGUID( int32_t guid )
{
    mActivatorGUID = guid;
}

int32_t ActivatableComponent::GetActivatorGUID() const
{
    return mActivatorGUID;
}

bool ActivatableComponent::IsActivated() const
{
    return mActivatorGUID != -1;
}

void ActivatableComponent::SetHighlighted( bool highlighted )
{
    mHighlighted = highlighted;
}

bool ActivatableComponent::IsHighlighted() const
{
    return mHighlighted;
}



void ActivatableComponentLoader::BindValues()
{
}

ActivatableComponentLoader::ActivatableComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( ActivatableComponent, ActivatableComponent );
