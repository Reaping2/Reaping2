#ifndef INCLUDED_CORE_WEAPON_ASSET_H
#define INCLUDED_CORE_WEAPON_ASSET_H
#include "core/item.h"
#include "core/actor_factory.h"

class Weapon : public Item
{
public:
    bool IsShoot() const;
    void SetShoot(bool shoot);
    bool IsShootAlt() const;
    void SetShootAlt(bool shoot);
    double GetCooldown() const;
    void SetCooldown(double cooldown);
    double GetShootCooldown() const;
    void SetShootCooldown(double cooldown);
    double GetShootAltCooldown() const;
    void SetShootAltCooldown(double cooldown);
    uint32_t GetScatter() const;
    void SetScatter(uint32_t scatter);
    uint32_t GetAltScatter() const;
    void SetAltScatter(uint32_t scatter);
    Weapon( int32_t Id );

    ActorFactory& mActorFactory;
    double mCooldown;
    double mShootCooldown;
    double mShootAltCooldown;
    uint32_t mScatter;
    uint32_t mAltScatter;
    bool mShoot;
    bool mShootAlt;
    friend class ItemFactory;
};

#endif//INCLUDED_CORE_WEAPON_ASSET_H
