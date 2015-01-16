#include "platform/i_platform.h"
#include "core/plasma_gun.h"
#include "core/audible_event.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"

PlasmaGun::PlasmaGun( int32_t Id )
    : Weapon( Id )
{
    mScatter = 10;
    mAltScatter = 3;
    mShootCooldown = 0.1;
    mShootAltCooldown = 0.3;
}