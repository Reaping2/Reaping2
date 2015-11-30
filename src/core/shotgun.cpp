#include "platform/i_platform.h"
#include "core/shotgun.h"

Shotgun::Shotgun( int32_t Id )
    : Weapon( Id )
{
    mScatter.mIncrease=30;
    mScatter.mChill=40;
    mScatter.mAltIncrease=10;
    mShootCooldown = 0.5;
    mShootAltCooldown = 1.5;
}
