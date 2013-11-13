#include "i_core.h"

ShootAction::ShootAction()
	:Action("shoot")
{
	mAreBlockedActionsExcluded=false;
	mCancelledActionIds.push_back(AutoId("shoot_alt"));
	mSecsToEnd=1;
	mIsLoop=true;
	mIsSelfDestruct=false;
	mIsRefresh=false;
}

void ShootAction::Update( Actor& Actor,double Seconds ) 
{
	Action::Update(Actor,Seconds);
	Action const* weapon = Actor.GetWeapon();
	if(!weapon)
		return;

	WeaponAsset const * weaponAsset = static_cast<WeaponAsset const*>(weapon);
	weaponAsset->Shoot(Actor);
}
ShootAltAction::ShootAltAction()
	:Action("shoot_alt")
{
	mAreBlockedActionsExcluded=false;
	mCancelledActionIds.push_back(AutoId("shoot"));
	mSecsToEnd=1;
	mIsLoop=true;
	mIsSelfDestruct=false;
	mIsRefresh=false;
}

void ShootAltAction::Update( Actor& Actor,double Seconds ) 
{
	Action::Update(Actor,Seconds);
	Action const* weapon = Actor.GetWeapon();
	if(!weapon)
		return;

	WeaponAsset const * weaponAsset = static_cast<WeaponAsset const*>(weapon);
	weaponAsset->ShootAlt(Actor);
}