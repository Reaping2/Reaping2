#ifndef INCLUDED_CORE_PLASMA_GUN_ACTION_H
#define INCLUDED_CORE_PLASMA_GUN_ACTION_H

class PlasmaGunAction : public WeaponAsset
{
public:	
	virtual void Shoot();
	virtual void ShootAlt();
protected:
	PlasmaGunAction(int32_t Id);
	virtual void Update(double Seconds)  override;
	friend class Factory<Item>;
};

#endif//INCLUDED_CORE_PLASMA_GUN_ACTION_H
