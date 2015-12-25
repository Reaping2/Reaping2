#ifndef INCLUDED_CORE_ION_GUN_H
#define INCLUDED_CORE_ION_GUN_H
#include "core/weapon.h"

class IonGun : public Weapon
{
    IonGun( int32_t Id );
    friend class Factory<Item>;
};

#endif//INCLUDED_CORE_ION_GUN_H
