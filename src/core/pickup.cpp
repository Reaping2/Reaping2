#include "i_core.h"

Pickup::Pickup( std::string const& Name )
:Actor(Name)
{
	mFields[COLLISION_CLASS].i=CollisionClass::Pickup;
	mFields[RADIUS].d=0.04;
	AddAction(AutoId("fade_out"));
}

void Pickup::Collide( Actor& Other )
{
	Other.DropItemType(Item::Weapon);
	Other.AddItem(GetId());
	mFields[HP].i=HP_DEAD;
}

void Pickup::UpdateLifetime()
{
	if(!IsAlive())
		delete this;
}
