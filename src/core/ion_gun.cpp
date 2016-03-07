#include "core/ion_gun.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

IonGun::IonGun( int32_t Id )
    : Weapon( Id )
{
}

IonGun::IonGun()
    : Weapon( -1 )
{
}

void IonGunLoader::BindValues()
{

}

IonGunLoader::IonGunLoader()
{
    SetBase<WeaponLoader>();
}

REAPING2_CLASS_EXPORT_IMPLEMENT( IonGun, IonGun );
