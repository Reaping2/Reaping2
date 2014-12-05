#ifndef INCLUDED_CORE_WEAPON_ASSET_H
#define INCLUDED_CORE_WEAPON_ASSET_H
#include "core/item.h"
#include "core/actor_factory.h"

class Weapon : public Item
{
public:
    virtual void Shoot();
    virtual void ShootAlt();
protected:
    Weapon( int32_t Id );
    typedef boost::ptr_vector<Actor> Projectiles_t;
    virtual void ShootImpl( Projectiles_t& Projectiles ) = 0;
    virtual void ShootAltImpl( Projectiles_t& Projectiles ) = 0;
    virtual void Update( double Seconds );

    ActorFactory& mActorFactory;
    double mCooldown;
    double mShootCooldown;
    double mShootAltCooldown;
    uint32_t mScatter;
    uint32_t mAltScatter;
    friend class ItemFactory;
};

#endif//INCLUDED_CORE_WEAPON_ASSET_H
