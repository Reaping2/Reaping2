#include "platform/i_platform.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/i_health_component.h"
#include "core/target_player_controller_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

TargetPlayerControllerComponent::TargetPlayerControllerComponent()
    : ControllerComponent()
    , mAttackCounter( 0.0 )
    , mAttackFrequency( 2.0 )
    , mHeadingModifierCounter ( 0.0 )
    , mHeadingModifierFrequency ( 0.2 )
    , mHeadingModifier( 0 )
    , mNeedInit( true )
{

}



double TargetPlayerControllerComponent::GetHeadingModifier() const
{
    return mHeadingModifier;
}

void TargetPlayerControllerComponent::SetHeadingModifier(double headingModifier)
{
    mHeadingModifier = headingModifier;
}

double TargetPlayerControllerComponent::GetAttackCounter() const
{
    return mAttackCounter;
}

void TargetPlayerControllerComponent::SetAttackCounter(double counter)
{
    mAttackCounter = counter;
}

double TargetPlayerControllerComponent::GetHeadingModifierCounter() const
{
    return mHeadingModifierCounter;
}

void TargetPlayerControllerComponent::SetHeadingModifierCounter(double counter)
{
    mHeadingModifierCounter=counter;
}

double TargetPlayerControllerComponent::GetAttackFrequency() const
{
    return mAttackFrequency;
}

void TargetPlayerControllerComponent::SetAttackFrequency(double frequency)
{
    mAttackFrequency=frequency;
}

double TargetPlayerControllerComponent::GetHeadingModifierFrequency() const
{
    return mHeadingModifierFrequency;
}

void TargetPlayerControllerComponent::SetHeadingModifierFrequency(double frequency)
{
    mHeadingModifierFrequency=frequency;
}

bool TargetPlayerControllerComponent::IsNeedInit() const
{
    return mNeedInit;
}

void TargetPlayerControllerComponent::SetNeedInit(bool needInit)
{
    mNeedInit=needInit;
}



void TargetPlayerControllerComponentLoader::BindValues()
{
    Bind("attack_frequency",func_double(&TargetPlayerControllerComponent::SetAttackFrequency));
    Bind("heading_modifier_frequency",func_double(&TargetPlayerControllerComponent::SetHeadingModifierFrequency));
}

TargetPlayerControllerComponentLoader::TargetPlayerControllerComponentLoader()
{

}

BOOST_CLASS_EXPORT_IMPLEMENT(TargetPlayerControllerComponent);
