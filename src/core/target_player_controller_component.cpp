#include "platform/i_platform.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/i_health_component.h"
#include "core/target_player_controller_component.h"
#include "core/action_repo.h"

TargetPlayerControllerComponent::TargetPlayerControllerComponent()
    : ControllerComponent()
    , mPlayer( NULL )
    , mCounter( 0.0 )
    , mHeadingModifier( 0 )
{

}

void TargetPlayerControllerComponent::SetPlayer( Actor* Player )
{
    mPlayer = Player;
}

Actor* TargetPlayerControllerComponent::GetPlayer()
{
    return mPlayer;
}

double TargetPlayerControllerComponent::GetHeadingModifier() const
{
    return mHeadingModifier;
}

void TargetPlayerControllerComponent::SetHeadingModifier(double headingModifier)
{
    mHeadingModifier = headingModifier;
}

double TargetPlayerControllerComponent::GetCounter() const
{
    return mCounter;
}

void TargetPlayerControllerComponent::SetCounter(double counter)
{
    mCounter = counter;
}



void TargetPlayerControllerComponentLoader::BindValues()
{

}

TargetPlayerControllerComponentLoader::TargetPlayerControllerComponentLoader()
{

}
