#ifndef INCLUDED_CORE_PLASMA_GUN_ACTION_H
#define INCLUDED_CORE_PLASMA_GUN_ACTION_H

class PlasmaGun : public Weapon
{
    PlasmaGun( int32_t Id );
    void ShootImpl( Projectiles_t& Projectiles );
    void ShootAltImpl( Projectiles_t& Projectiles );
    friend class Factory<Item>;
    int32_t mShotId;
    int32_t mAltShotId;
};

#endif//INCLUDED_CORE_PLASMA_GUN_ACTION_H
