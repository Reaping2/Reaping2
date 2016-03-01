#include "platform/i_platform.h"
#include "core/random_controller_component.h"
#include "core/i_move_component.h"
#include "core/i_health_component.h"
#include "core/i_position_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

RandomControllerComponent::RandomControllerComponent()
    : ControllerComponent()
    , mCounter( 2 )
    , mHeadingModifier( 0 )
{

}

double RandomControllerComponent::GetCounter() const
{
    return mCounter;
}

void RandomControllerComponent::SetCounter(double counter)
{
    mCounter = counter;
}

double RandomControllerComponent::GetHeadingModifier() const
{
    return mHeadingModifier;
}

void RandomControllerComponent::SetHeadingModifier(double headingModifier)
{
    mHeadingModifier = headingModifier;
}


void RandomControllerComponentLoader::BindValues()
{

}

RandomControllerComponentLoader::RandomControllerComponentLoader()
{

}

REAPING2_CLASS_EXPORT_IMPLEMENT(RandomControllerComponent, RandomControllerComponent);
