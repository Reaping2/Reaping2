#include "core/stop_on_death_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

StopOnDeathComponent::StopOnDeathComponent()
    : mStopped( false )
{
}

void StopOnDeathComponent::SetStopped( bool stopped )
{
    mStopped = stopped;
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

REAPING2_CLASS_EXPORT_IMPLEMENT( StopOnDeathComponent, StopOnDeathComponent );
