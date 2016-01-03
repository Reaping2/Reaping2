#include "platform/i_platform.h"
#include "core/gauss_gun.h"
#include "core/audible_event.h"
#include "core/i_move_component.h"

GaussGun::GaussGun( int32_t Id )
    : Weapon( Id )
{
    mScatter.mIncrease=15;
    mScatter.mChill=40;
    mScatter.mAltIncrease=220;
    mScatter.mMagicNumber=100;
    mShootCooldown = 1.9;
    mShootAltCooldown = 0.7;
    mBulletsMax = 6.0;
    mShotCost=1;
    mShotCostAlt=1;
    mReloadTimeMax=1.7;
    mBullets=mBulletsMax;
}

