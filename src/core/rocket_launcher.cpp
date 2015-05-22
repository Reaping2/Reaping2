#include "core/rocket_launcher.h"

RocketLauncher::RocketLauncher( int32_t Id )
	: Weapon( Id )
{
	mScatter = 20;
	mAltScatter = 10;
	mShootCooldown = 0.25;
	mShootAltCooldown = 0.5;
}