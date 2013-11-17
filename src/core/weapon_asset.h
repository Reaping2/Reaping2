#ifndef INCLUDED_CORE_WEAPON_ASSET_H
#define INCLUDED_CORE_WEAPON_ASSET_H

class WeaponAsset : public Item
{
public:	
	virtual void Shoot() =0;
	virtual void ShootAlt()=0;
protected:
	double mCooldownReduction;
	double mCooldown;
	WeaponAsset(int32_t Id);
	virtual void Update(double Seconds)  override;
	friend class Factory<Action>;
};

#endif//INCLUDED_CORE_WEAPON_ASSET_H
