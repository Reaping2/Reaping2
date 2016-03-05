#include "platform/i_platform.h"
#include "core/remove_on_death_component.h"
#include "core/i_health_component.h"
#include "core/actor.h"
#include <boost/assert.hpp>
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

RemoveOnDeathComponent::RemoveOnDeathComponent()
    : mRemainingTime( 0 )
{

}

RemoveOnDeathComponentLoader::RemoveOnDeathComponentLoader()
{

}

void RemoveOnDeathComponentLoader::BindValues()
{
    Bind("delay",func_double(&RemoveOnDeathComponent::SetRemainingTime));
}

void RemoveOnDeathComponent::SetRemainingTime( double rt )
{
    mRemainingTime = rt;
}

double RemoveOnDeathComponent::GetRemainingTime() const
{
    return mRemainingTime;
}

REAPING2_CLASS_EXPORT_IMPLEMENT(RemoveOnDeathComponent, RemoveOnDeathComponent);
