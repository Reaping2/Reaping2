#include "core/rocket_launcher.h"

RocketLauncher::RocketLauncher( int32_t Id )
	: Weapon( Id )
{
    InitMembers();
}

RocketLauncher::RocketLauncher()
    : Weapon(-1)
{
    InitMembers();
}

void RocketLauncher::InitMembers()
{
    mScatter.mIncrease=22;//25;
    mScatter.mChill=31;
    mScatter.mAltIncrease=5;//5;
    mShootCooldown = 0.35;
    mShootAltCooldown = 1.5;

    mBulletsMax = 12;//10.0;
    mShotCost=1;
    mShotCostAlt=2;
    mReloadTimeMax=3.0;
    mBullets=mBulletsMax;
}
