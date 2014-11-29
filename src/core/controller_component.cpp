#include "core/controller_component.h"

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
