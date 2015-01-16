#include "platform/i_platform.h"
#include "core/pistol.h"
#include "core/audible_event.h"
#include "core/i_move_component.h"

Pistol::Pistol( int32_t Id )
    : Weapon( Id )
{
    mShootCooldown = 0.2;
    mShootAltCooldown = 0.1;
    mScatter = 10;
    mAltScatter = 30;
}
