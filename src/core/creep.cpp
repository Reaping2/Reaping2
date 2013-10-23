#include "i_core.h"

Creep::Creep()
{
	mFields[COLLISION_CLASS].i=CollisionClass::Creep;
	SetController(std::auto_ptr<Controller>(new RandomController));
}

