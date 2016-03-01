#include "core/plasma_gun.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

PlasmaGun::PlasmaGun( int32_t Id )
    : Weapon( Id )
{
    InitMembers();

}

PlasmaGun::PlasmaGun()
    : Weapon(-1)
{
    InitMembers();
}

void PlasmaGun::InitMembers()
{
    mScatter.mIncrease=13;
    mScatter.mChill=40;
    mScatter.mAltIncrease=20;

    mShootCooldown = 0.15;
    mShootAltCooldown = 0.35;

    mBulletsMax = 200.0;
    mShotCost=10;
    mShotCostAlt=30;
    mReloadTimeMax=0.2;
    mStaticReload=5;
    mBullets=mBulletsMax;
}

REAPING2_CLASS_EXPORT_IMPLEMENT(PlasmaGun, PlasmaGun);
