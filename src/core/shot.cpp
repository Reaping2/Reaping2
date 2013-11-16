#include "i_core.h"

Shot::Shot(std::string const& Name)
	:Actor(Name)
{
	mFields[COLLISION_CLASS].i=CollisionClass::Projectile;
	mFields[RADIUS].d=0.06;
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
