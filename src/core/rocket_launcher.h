#ifndef INCLUDED_CORE_ROCKET_LAUNCHER_H
#define INCLUDED_CORE_ROCKET_LAUNCHER_H
#include "core/weapon.h"

class RocketLauncher :	public Weapon
{
	RocketLauncher(int32_t Id);
	friend class Factory<Item>;
};

#endif // INCLUDED_CORE_ROCKET_LAUNCHER_H
