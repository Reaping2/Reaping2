#include "core/rocket_launcher.h"

RocketLauncher::RocketLauncher( int32_t Id )
	: Weapon( Id )
{
    mScatter.mIncrease=20;
    mScatter.mChill=40;
    mScatter.mAltIncrease=5;
	mShootCooldown = 0.35;
	mShootAltCooldown = 1.5;
}