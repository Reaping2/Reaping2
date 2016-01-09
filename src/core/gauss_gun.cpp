#include "platform/i_platform.h"
#include "core/gauss_gun.h"
#include "core/audible_event.h"
#include "core/i_move_component.h"

GaussGun::GaussGun( int32_t Id )
    : Weapon( Id )
    , mChargeTime( 0.3 )
{
    mScatter.mIncrease=15;
    mScatter.mChill=40;
    mScatter.mAltIncrease=220;
    mScatter.mMagicNumber=100;
    mShootCooldown = 1.3;
    mShootAltCooldown = 1.9;
    mBulletsMax = 6.0;
    mShotCost=1;
    mShotCostAlt=1;
    mReloadTimeMax=1.7;
    mBullets=mBulletsMax;
}

glm::vec3 GaussGun::GetMouseColor() const
{
    if( GetCooldown() <= 0.0 && IsCharging() )
    {
        double state = std::max( 0.0, std::min( 1.0, ( glfwGetTime() - mCurrentCharge ) / mChargeTime ) );
        return glm::vec3( state, state, 1.0 - state );
    }
    return Weapon::GetMouseColor();
}

void GaussGun::StartCharge()
{
    if( mCurrentCharge >= 0.0 )
        return;
    mCurrentCharge = glfwGetTime();
}

void GaussGun::EndCharge()
{
    mCurrentCharge = -1.;
}

bool GaussGun::IsShootingAlt() const
{
    return Weapon::IsShootingAlt() &&
        mCurrentCharge > 0.0 &&
        glfwGetTime() - mCurrentCharge >= mChargeTime;

}

bool GaussGun::IsCharging() const
{
    return mCurrentCharge > 0.0;
}

