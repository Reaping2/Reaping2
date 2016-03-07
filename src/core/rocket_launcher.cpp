#include "core/rocket_launcher.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

RocketLauncher::RocketLauncher( int32_t Id )
    : Weapon( Id )
{
}

RocketLauncher::RocketLauncher()
    : Weapon( -1 )
{
}

void RocketLauncherLoader::BindValues()
{
}

RocketLauncherLoader::RocketLauncherLoader()
{
    SetBase<WeaponLoader>();
}

REAPING2_CLASS_EXPORT_IMPLEMENT( RocketLauncher, RocketLauncher );
