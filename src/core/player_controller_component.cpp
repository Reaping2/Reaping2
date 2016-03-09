#include "platform/i_platform.h"
#include "input/i_input.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/player_controller_component.h"
#include "core/i_health_component.h"
#include "core/scene.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

PlayerControllerComponent::PlayerControllerComponent()
    : ControllerComponent()
    , mOrientation( 0.0 )
    , mHeading( 0.0 )
    , mShoot( false )
    , mShootAlt( false )
    , mUseNormalItem()
    , mActive( false )
    , mControllerId( 0 )
    , mUseReload()
    , mMoving( false )
{
}


void PlayerControllerComponentLoader::BindValues()
{

}

PlayerControllerComponentLoader::PlayerControllerComponentLoader()
{

}

REAPING2_CLASS_EXPORT_IMPLEMENT( PlayerControllerComponent, PlayerControllerComponent );
