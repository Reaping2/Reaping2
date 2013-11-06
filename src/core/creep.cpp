#include "i_core.h"

Creep::Creep(std::string const& Name, double x,double y,Actor* player)
	:Actor(Name)
{
	mFields[X].d=x;
	mFields[Y].d=y;
	mFields[COLLISION_CLASS].i=CollisionClass::Creep;
	// ez nem innen fog jonni, de kb itt kell beallitani
	// a string ctor param lesz
	// player param nyilvan eltunik
	//mFields[TYPE_ID].i=IdStr.GetId(rand()%2?"pok1":"pok2");
	//mFields[ACTION_ID].i=IdStr.GetId("idle");
	Controller* Ctrl=player?(Controller*)(new TargetPlayerController(player)):(Controller*)new RandomController;
	SetController(std::auto_ptr<Controller>(Ctrl));
}