#ifndef INCLUDED_CORE_WEAPON_ASSET_H
#define INCLUDED_CORE_WEAPON_ASSET_H

class WeaponAsset : public Item
{
public:
    virtual void Shoot();
    virtual void ShootAlt();
protected:
    typedef boost::ptr_vector<Shot> Projectiles_t;
    virtual void ShootImpl( Projectiles_t& Projectiles ) = 0;
    virtual void ShootAltImpl( Projectiles_t& Projectiles ) = 0;
    double mCooldown;
    double mShootCooldown;
    double mShootAltCooldown;
    uint32_t mScatter;
    uint32_t mAltScatter;
    WeaponAsset( int32_t Id );
    virtual void Update( double Seconds );
    friend class Factory<Action>;
};

#endif//INCLUDED_CORE_WEAPON_ASSET_H
