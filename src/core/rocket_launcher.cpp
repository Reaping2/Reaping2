#include "core/rocket_launcher.h"

RocketLauncher::RocketLauncher( int32_t Id )
	: Weapon( Id )
{
	mScatter = 20;
	mAltScatter = 5;
	mShootCooldown = 0.25;
	mShootAltCooldown = 1.5;
}