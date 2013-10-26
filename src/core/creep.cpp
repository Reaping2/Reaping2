#include "i_core.h"

Creep::Creep( double x, double y )
{
	mFields[X].d=x;
	mFields[Y].d=y;
	mFields[COLLISION_CLASS].i=CollisionClass::Creep;
	SetController(std::auto_ptr<Controller>(new RandomController));
}

