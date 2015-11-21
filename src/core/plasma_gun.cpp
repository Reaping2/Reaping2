#include "core/plasma_gun.h"

PlasmaGun::PlasmaGun( int32_t Id )
    : Weapon( Id )
{
    mScatter = 12;
    mAltScatter = 10;
    mShootCooldown = 0.15;
    mShootAltCooldown = 0.35;
}