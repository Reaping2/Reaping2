#include "core/trigger.h"

Trigger::Trigger()
    : mActive( false )
    , mHandled( true )
    , mTriggered( false )
{
}

void Trigger::Activate()
{
    mActive = true;
    if (mHandled&&!mTriggered)
    {
        mHandled = false;
        mTriggered = true;
    }
}

void Trigger::Deactivate()
{
    mActive = false;
    if (mHandled)
    {
        mTriggered = false;
    }
}

void Trigger::Handled()
{
    mHandled = true;
    if (!mActive)
    {
        mTriggered = false;
    }
}

bool Trigger::GetValue() const
{
    return !mHandled && mTriggered;
}

bool Trigger::IsActive() const
{
    return mActive;
}

