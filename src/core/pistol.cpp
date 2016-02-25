#include "platform/i_platform.h"
#include "core/pistol.h"
#include "core/audible_event.h"
#include "core/i_move_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

Pistol::Pistol( int32_t Id )
    : Weapon( Id )
{
    InitMembers();

}

Pistol::Pistol()
    : Weapon( -1 )
{
    InitMembers();
}

void Pistol::InitMembers()
{
    mScatter.mIncrease=20;
    mScatter.mChill=30;
    mScatter.mAltIncrease=20;
    mScatter.mMagicNumber=100;
    mShootCooldown = 1.2;
    mShootAltCooldown = 0.3;
    mBulletsMax = 26.0;
    mShotCost=1;
    mShotCostAlt=1;
    mReloadTimeMax=1.3;
    mBullets=mBulletsMax;
}

BOOST_CLASS_EXPORT_IMPLEMENT(Pistol);
