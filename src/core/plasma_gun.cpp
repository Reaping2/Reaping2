#include "core/plasma_gun.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

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

BOOST_CLASS_EXPORT_IMPLEMENT(PlasmaGun);
