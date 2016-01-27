#ifndef INCLUDED_CORE_SHOTGUN_H
#define INCLUDED_CORE_SHOTGUN_H
#include "core/weapon.h"
#include "i_explode.h"

class Shotgun : public Weapon, public IExplode
{
    Shotgun( int32_t Id );
    friend class Factory<Item>;
};

#endif//INCLUDED_CORE_SHOTGUN_H
