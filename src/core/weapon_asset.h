#ifndef INCLUDED_CORE_WEAPON_ASSET_H
#define INCLUDED_CORE_WEAPON_ASSET_H

class WeaponAsset : public Action
{
public:	
	virtual void Shoot() const=0;
	virtual void ShootAlt() const=0;
protected:
	double mCooldownReduction;
	WeaponAsset(int32_t Id);
	virtual void Update(double Seconds)  override;
	friend class Factory<Action>;
};

#endif//INCLUDED_CORE_WEAPON_ASSET_H
