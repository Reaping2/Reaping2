#ifndef INCLUDED_CORE_WEAPON_ASSET_H
#define INCLUDED_CORE_WEAPON_ASSET_H

class WeaponAsset : public Action
{
public:	
	virtual void Shoot(Actor& Actor) const=0;
	virtual void ShootAlt(Actor& Actor) const=0;
protected:
	double mCooldownReduction;
	WeaponAsset(std::string const& Name);
	virtual void Update(Actor& Actor,double Seconds) const override;
	friend class ActionRepo;
};

#endif//INCLUDED_CORE_WEAPON_ASSET_H
