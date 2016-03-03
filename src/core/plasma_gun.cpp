#include "core/plasma_gun.h"

PlasmaGun::PlasmaGun( int32_t Id )
    : Weapon( Id )
{
}

PlasmaGun::PlasmaGun()
    : Weapon(-1)
{
}

void PlasmaGunLoader::BindValues()
{
}

PlasmaGunLoader::PlasmaGunLoader()
{
    SetBase<WeaponLoader>();
}
