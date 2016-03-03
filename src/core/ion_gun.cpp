#include "core/ion_gun.h"

IonGun::IonGun( int32_t Id )
    : Weapon( Id )
{
}

IonGun::IonGun()
    : Weapon(-1)
{
}

void IonGunLoader::BindValues()
{

}

IonGunLoader::IonGunLoader()
{
    SetBase<WeaponLoader>();
}
