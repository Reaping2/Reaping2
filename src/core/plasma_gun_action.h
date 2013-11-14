#ifndef INCLUDED_CORE_PLASMA_GUN_ACTION_H
#define INCLUDED_CORE_PLASMA_GUN_ACTION_H

class PlasmaGunAction : public WeaponAsset
{
public:	
	virtual void Shoot() const;
	virtual void ShootAlt() const;
protected:
	PlasmaGunAction(int32_t Id, Actor& actor);
	virtual void Update(double Seconds)  override;
	friend class ActionRepo;
};

#endif//INCLUDED_CORE_PLASMA_GUN_ACTION_H
