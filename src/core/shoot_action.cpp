#include "i_core.h"

ShootAction::ShootAction(int32_t Id, Actor& actor)
	: Action(Id,actor)
{
	mAreBlockedActionsExcluded=false;
	mCancelledActionIds.push_back(AutoId("shoot_alt"));
	mSecsToEnd=1;
	mIsLoop=true;
	mIsSelfDestruct=false;
	mIsRefresh=false;
}

void ShootAction::Update(double Seconds) 
{
	Action::Update(Seconds);
	Action const* weapon = mActor.GetWeapon();
	if(!weapon)
		return;

	WeaponAsset const * weaponAsset = static_cast<WeaponAsset const*>(weapon);
	weaponAsset->Shoot();
}
ShootAltAction::ShootAltAction(int32_t Id, Actor& actor)
	: Action(Id,actor)
{
	mAreBlockedActionsExcluded=false;
	mCancelledActionIds.push_back(AutoId("shoot"));
	mSecsToEnd=1;
	mIsLoop=true;
	mIsSelfDestruct=false;
	mIsRefresh=false;
}

void ShootAltAction::Update(double Seconds) 
{
	Action::Update(Seconds);
	Action const* weapon = mActor.GetWeapon();
	if(!weapon)
		return;

	WeaponAsset const * weaponAsset = static_cast<WeaponAsset const*>(weapon);
	weaponAsset->ShootAlt();
}