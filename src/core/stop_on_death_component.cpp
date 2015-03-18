#include "core/stop_on_death_component.h"

StopOnDeathComponent::StopOnDeathComponent()
    : mStopped(false)
{
}

void StopOnDeathComponent::SetStopped(bool stopped)
{
    mStopped=stopped;
}

bool StopOnDeathComponent::IsStopped()const
{
    return mStopped;
}



void StopOnDeathComponentLoader::BindValues()
{
}

StopOnDeathComponentLoader::StopOnDeathComponentLoader()
{
}
