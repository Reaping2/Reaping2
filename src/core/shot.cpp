#include "i_core.h"

Shot::Shot(std::string const& Name)
	:Actor(Name)
{
	mFields[COLLISION_CLASS].i=CollisionClass::Projectile;
}