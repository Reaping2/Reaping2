#include "i_core.h"

Creep::Creep(std::string const& Name, double x,double y,Actor* player)
	:Actor(Name)
{
	mFields[X].d=x;
	mFields[Y].d=y;
	mFields[RADIUS].d=0.1;
	mFields[COLLISION_CLASS].i=CollisionClass::Creep;
	mFields[HP].i=10;
	// ez nem innen fog jonni, de kb itt kell beallitani
	// a string ctor param lesz
	// player param nyilvan eltunik
	//mFields[TYPE_ID].i=IdStr.GetId(rand()%2?"pok1":"pok2");
	//mFields[ACTION_ID].i=IdStr.GetId("idle");
	Controller* Ctrl=player?(Controller*)(new TargetPlayerController(player)):(Controller*)new RandomController;
	SetController(std::auto_ptr<Controller>(Ctrl));
}

void Creep::OnDeath()
{
#ifdef DEBUG
	static const size_t Mod=3;
#else
	static const size_t Mod=10;
#endif//DEBUG
	if(rand()%Mod)
		return;

	Pickup* Pu=new Pickup(rand()%2?"pistol":"plasma_gun");
	Pu->SetX(GetX());
	Pu->SetY(GetY());
	Scene::Get().AddActor(Pu);
}
