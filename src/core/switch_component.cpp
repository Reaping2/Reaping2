#include "core/switch_component.h"

SwitchComponent::SwitchComponent()
    : mState(SwitchState::Off)
    , mSecsToEnd(0.0)
    , mSecsToEndMax( 0.0 )
    , mStateChanged(SwitchState::Off)
{
}

void SwitchComponent::SetState( SwitchState::Type state )
{
    if (mState != state)
    {
        mStateChanged = true;
    }
    mState = state;
}

SwitchState::Type SwitchComponent::GetState() const
{
    return mState;
}

void SwitchComponent::SetSecsToEnd( double secsToEnd )
{
    mSecsToEnd = secsToEnd;
}

double SwitchComponent::GetSecsToEnd() const
{
    return mSecsToEnd;
}

void SwitchComponent::SetSecsToEndMax( double secsToEndMax )
{
    mSecsToEndMax = secsToEndMax;
}

double SwitchComponent::GetSecsToEndMax() const
{
    return mSecsToEndMax;
}


void SwitchComponent::SetStateChanged( bool stateChanged )
{
    mStateChanged = stateChanged;
}

bool SwitchComponent::IsStateChanged() const
{
    return mStateChanged;
}



void SwitchComponentLoader::BindValues()
{
    Bind( "secs_to_end", func_double( &SwitchComponent::SetSecsToEndMax ) );
}

SwitchComponentLoader::SwitchComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( SwitchComponent, SwitchComponent );
