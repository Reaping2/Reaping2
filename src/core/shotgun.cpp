#include "platform/i_platform.h"
#include "core/shotgun.h"

Shotgun::Shotgun( int32_t Id )
    : Weapon( Id )
{
    mScatter = 10;
    mAltScatter = 10;
    mShootCooldown = 0.5;
    mShootAltCooldown = 0.5;
}
