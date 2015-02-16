#include "core/rocket_launcher.h"

RocketLauncher::RocketLauncher( int32_t Id )
	: Weapon( Id )
{
	mScatter = 0;
	mAltScatter = 2;
	mShootCooldown = 0.2;
	mShootAltCooldown = 0.4;
}