#ifndef INCLUDED_CORE_SHOTGUN_H
#define INCLUDED_CORE_SHOTGUN_H
#include "core/weapon.h"

class Shotgun : public Weapon
{
    Shotgun( int32_t Id );
    friend class Factory<Item>;
};

#endif//INCLUDED_CORE_SHOTGUN_H
