#include "core/lucky_rocket.h"

LuckyRocket::LuckyRocket( int32_t Id )
    : Weapon( Id )
{
    mScatter.mIncrease=100;
    mScatter.mChill=40;
    mScatter.mAltIncrease=100;
    mShootCooldown = 0.15;
    mShootAltCooldown = 0.15;

    mBulletsMax = 10.0;
    mShotCost=0;
    mShotCostAlt=0;
    mReloadTimeMax=3.0;
    mBullets=mBulletsMax;
}

glm::vec3 LuckyRocket::GetMouseColor() const
{
    double t = glfwGetTime();
    return mReloadTime>0.0&&mStaticReload==0.0?
        glm::vec3( 1.0, 0.0, 0.0 ):glm::vec3( sin( t ) * sin( t ) / 2., cos( t ) * cos( t ), 1 - std::abs( sin( t ) ) );
}

std::string LuckyRocket::GetMouseText() const
{
    return mReloadTime>0.0 ? "Not so lucky!" : Weapon::GetMouseText();
}


