#include "core/rocket_launcher.h"

RocketLauncher::RocketLauncher( int32_t Id )
	: Weapon( Id )
{
}

RocketLauncher::RocketLauncher()
    : Weapon(-1)
{
}

void RocketLauncherLoader::BindValues()
{
}

RocketLauncherLoader::RocketLauncherLoader()
{
    SetBase<WeaponLoader>();
}
