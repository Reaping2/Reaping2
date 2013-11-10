#include "i_core.h"

WeaponAsset::WeaponAsset(std::string const& Name)
: Action(Name)
,mCooldownReduction(0.0)
{
	mType=Weapon;
}
void WeaponAsset::Update(Actor& Actor, double Seconds) const
{
	Action::Update(Actor,Seconds);
	//double cd = Actor.GetWeaponCooldown();
	//cd-=mCooldownReduction*Seconds;
	//if(cd<0)cd=0;
	//Actor.SetWeaponCooldown(cd);
}