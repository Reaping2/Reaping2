#include "core/rocket_launcher.h"

RocketLauncher::RocketLauncher( int32_t Id )
	: Weapon( Id )
{
	mScatter = 12;
	mAltScatter = 5;
	mShootCooldown = 0.35;
	mShootAltCooldown = 1.5;
}