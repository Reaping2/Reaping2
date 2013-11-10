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

void ShootAction::Update( Actor& Actor,double Seconds ) const
{
	Action::Update(Actor,Seconds);
	Actor::ActionDesc_t const* weapon = Actor.GetWeapon();
	if(!weapon)
		return;

	WeaponAsset const * weaponAsset = static_cast<WeaponAsset const*>(weapon->GetAction());
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

void ShootAltAction::Update( Actor& Actor,double Seconds ) const
{
	Action::Update(Actor,Seconds);
	Actor::ActionDesc_t const* weapon = Actor.GetWeapon();
	if(!weapon)
		return;

	WeaponAsset const * weaponAsset = static_cast<WeaponAsset const*>(weapon->GetAction());
	weaponAsset->ShootAlt(Actor);
}