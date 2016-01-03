#pragma once
#ifndef INCLUDED_GAUSS_GUN_H
#define INCLUDED_GAUSS_GUN_H

#include "core/weapon.h"

class GaussGun : public Weapon
{
    GaussGun( int32_t Id );
    friend class Factory<Item>;
};

#endif // INCLUDED_GAUSS_GUN_H

