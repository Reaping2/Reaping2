#include "i_core.h"

WeaponAsset::WeaponAsset(int32_t Id)
	: Item(Id)
	,mCooldownReduction(0.0)
	,mCooldown(0.0)
{
	mType=Weapon;
}
void WeaponAsset::Update( double Seconds) 
{
	Item::Update(Seconds);
	double cd = mCooldown;
	cd-=mCooldownReduction*Seconds;
	if(cd<0)cd=0;
	mCooldown=cd;
	if(!mActor)return;
	static const int32_t ShootId=AutoId("shoot");
	static const int32_t ShootAltId=AutoId("shoot_alt");
	if(mActor->HasAction(ShootId))
		Shoot();
	if(mActor->HasAction(ShootAltId))
		ShootAlt();
}