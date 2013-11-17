#include "i_core.h"

WeaponAsset::WeaponAsset(int32_t Id)
	: Action(Id)
	,mCooldownReduction(0.0)
{
	mType=Weapon;
}
void WeaponAsset::Update( double Seconds) 
{
	Action::Update(Seconds);
	//double cd = Actor.GetWeaponCooldown();
	//cd-=mCooldownReduction*Seconds;
	//if(cd<0)cd=0;
	//Actor.SetWeaponCooldown(cd);
}