#ifndef INCLUDED_CORE_PLASMA_GUN_H
#define INCLUDED_CORE_PLASMA_GUN_H
#include "core/weapon.h"

class PlasmaGun : public Weapon
{
    PlasmaGun( int32_t Id );
    friend class Factory<Item>;
};

#endif//INCLUDED_CORE_PLASMA_GUN_H
