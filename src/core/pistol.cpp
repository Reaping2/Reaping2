#include "platform/i_platform.h"
#include "core/pistol.h"
#include "core/audible_event.h"
#include "core/i_move_component.h"

Pistol::Pistol( int32_t Id )
    : Weapon( Id )
{
    mScatter.mIncrease=15;
    mScatter.mChill=40;
    mScatter.mAltIncrease=20;
    mScatter.mMagicNumber=100;
    mShootCooldown = 0.2;
    mShootAltCooldown = 0.1;
    mBulletsMax = 26.0;
    mShotCost=1;
    mShotCostAlt=1;
    mReloadTimeMax=1.3;
    mBullets=mBulletsMax;
}
