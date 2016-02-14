#include "core/ion_gun.h"

IonGun::IonGun( int32_t Id )
    : Weapon( Id )
{
    InitMembers();
}

IonGun::IonGun()
    : Weapon(-1)
{
    InitMembers();
}

void IonGun::InitMembers()
{
    mScatter.mIncrease=13;
    mScatter.mChill=40;
    mScatter.mAltIncrease=20;

    mShootCooldown = 0.75;
    mShootAltCooldown = 0.35;

    mBulletsMax = 21.0;
    mShotCost=2;
    mShotCostAlt=3;
    mReloadTimeMax=2.2;
    mBullets=mBulletsMax;
}
