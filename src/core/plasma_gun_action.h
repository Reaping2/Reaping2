#ifndef INCLUDED_CORE_PLASMA_GUN_ACTION_H
#define INCLUDED_CORE_PLASMA_GUN_ACTION_H

class PlasmaGunAction : public WeaponAsset
{
public:	
	virtual void Shoot(Actor& Actor) const;
	virtual void ShootAlt(Actor& Actor) const;
protected:
	PlasmaGunAction();
	virtual void Update(Actor& Actor,double Seconds) const override;
	friend class ActionRepo;
};

#endif//INCLUDED_CORE_PLASMA_GUN_ACTION_H
