#include "platform/i_platform.h"
#include "input/i_input.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/player_controller_component.h"
#include "core/i_health_component.h"
#include "core/scene.h"

PlayerControllerComponent::PlayerControllerComponent()
    : ControllerComponent()
{
}

void PlayerControllerComponent::Update( double Seconds )
{
}



void PlayerControllerComponentLoader::BindValues()
{

}

PlayerControllerComponentLoader::PlayerControllerComponentLoader()
{

}
