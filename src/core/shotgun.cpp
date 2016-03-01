#include "platform/i_platform.h"
#include "core/shotgun.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

Shotgun::Shotgun( int32_t Id )
    : Weapon( Id )
    , IExplode()
{
    InitMembers();
}

Shotgun::Shotgun()
    : Weapon( -1 )
    , IExplode()
{
    InitMembers();
}

void Shotgun::InitMembers()
{
    mScatter.mIncrease=30;
    mScatter.mChill=40;
    mScatter.mAltIncrease=10;
    mShootCooldown = 1.2;
    mShootAltCooldown = 1.8;

    mBulletsMax = 20.0;
    mShotCost=1;
    mShotCostAlt=2;
    mReloadTimeMax=2.0;
    mBullets=mBulletsMax;

    mCount=9;
    mDistribution=ExplodeDistributionType::Random;
    mSecsToEndVariance=0.2;
    mPositionVariance=50;
    mSpeedVariance=0.2;
    mExplosionScatter=0.35;
    mExplosionProjectile=AutoId("shotgun_projectile");
}

REAPING2_CLASS_EXPORT_IMPLEMENT(Shotgun, Shotgun);
