#include "i_core.h"

Shot::Shot(std::string const& Name)
	:Actor(Name)
{
	mFields[COLLISION_CLASS].i=CollisionClass::Projectile;
	mFields[RADIUS].d=0.02;
	mFields[DAMAGE].i=1;
	SetSpeed(2);
	AddAction(AutoId("move"));
}

void Shot::ClipScene()
{
	glm::vec4 const& AllowedDimensions=Scene::Get().GetDimensions();
	if(GetX()+GetRadius()<AllowedDimensions.x*2||
		GetX()-GetRadius()>AllowedDimensions.z*2||
		GetY()+GetRadius()<AllowedDimensions.y*2||
		GetY()-GetRadius()>AllowedDimensions.w*2)
		mFields[HP].i=HP_DEAD;
}

void Shot::Collide( Actor& Other )
{
	Other.TakeDamage(mFields[DAMAGE].i);
	mFields[HP].i=HP_DEAD;
}

void Shot::UpdateLifetime()
{
	if(!IsAlive())
		delete this;
}
