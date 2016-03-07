#include "core/trigger.h"

Trigger::Trigger( bool active/*=false*/ )
    : mActive( active )
    , mHandled( false )
{
}

void Trigger::SetActive( bool active )
{
    mActive = active;
    if ( !active )
    {
        SetHandled( false );
    }
}

bool Trigger::IsActive()const
{
    return mActive;
}

void Trigger::SetHandled( bool consumed )
{
    mHandled = consumed;
}

bool Trigger::IsHandled()const
{
    return mHandled;
}

bool Trigger::GetValue() const
{
    return !mHandled && mActive;
}
