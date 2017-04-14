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
    , mActivate()
    , mSwitchWeapon()
    , mSwitchNormalItem()
    , mActive( false )
    , mControllerId( 0 )
    , mUseReload()
    , mMoving( false )
    , mActivateRange( 200.0 )
    , mHighLightedGUID( -1 )
{
}


void PlayerControllerComponent::SetActivateRange( double activateRange )
{
    mActivateRange = activateRange;
}

void PlayerControllerComponentLoader::BindValues()
{
    Bind( "activate_range", func_double(&PlayerControllerComponent::SetActivateRange) );
}

PlayerControllerComponentLoader::PlayerControllerComponentLoader()
{

}

REAPING2_CLASS_EXPORT_IMPLEMENT( PlayerControllerComponent, PlayerControllerComponent );
