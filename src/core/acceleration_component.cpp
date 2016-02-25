#include "core/acceleration_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

AccelerationComponent::AccelerationComponent()
    : mAcceleration(0.0)
    , mMinSpeed(0.0)
    , mMaxSpeed(0.0)
{
}

double const& AccelerationComponent::GetAcceleration() const
{
    return mAcceleration;
}

double const& AccelerationComponent::GetMinSpeed() const
{
    return mMinSpeed;
}

double const& AccelerationComponent::GetMaxSpeed() const
{
    return mMaxSpeed;
}

void AccelerationComponent::SetAcceleration(double acceleration)
{
    mAcceleration=std::floor(acceleration*PRECISION)/PRECISION;
}

void AccelerationComponent::SetMinSpeed(double minSpeed)
{
    mMinSpeed=minSpeed;
}

void AccelerationComponent::SetMaxSpeed(double maxSpeed)
{
    mMaxSpeed=maxSpeed;
}


void AccelerationComponentLoader::BindValues()
{
    Bind("acceleration",func_double(&AccelerationComponent::SetAcceleration));
    Bind("min_speed",func_double(&AccelerationComponent::SetMinSpeed));
    Bind("max_speed",func_double(&AccelerationComponent::SetMaxSpeed));
}

AccelerationComponentLoader::AccelerationComponentLoader()
{

}

BOOST_CLASS_EXPORT_IMPLEMENT(AccelerationComponent);
