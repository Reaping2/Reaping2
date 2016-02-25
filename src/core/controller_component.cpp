#include "core/controller_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

void ControllerComponent::SetEnabled(bool Enabled)
{
    mEnabled=Enabled;
}

bool ControllerComponent::IsEnabled()
{
    return mEnabled;
}

ControllerComponent::ControllerComponent()
    : mEnabled(true)
{

}

void ControllerComponent::Update(double Seconds)
{

}

BOOST_CLASS_EXPORT_IMPLEMENT(ControllerComponent);
