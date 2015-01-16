#ifndef INCLUDED_CORE_PISTOL_H
#define INCLUDED_CORE_PISTOL_H
#include "core/weapon.h"

class Pistol : public Weapon
{
    Pistol( int32_t Id );
    friend class Factory<Item>;
};

#endif//INCLUDED_CORE_PISTOL_H
