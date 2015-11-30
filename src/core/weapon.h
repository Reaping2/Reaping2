#ifndef INCLUDED_CORE_WEAPON_ASSET_H
#define INCLUDED_CORE_WEAPON_ASSET_H
#include "core/item.h"
#include "core/actor_factory.h"

struct Scatter
{
    double mCurrent;
    double mIncrease;
    double mAltIncrease;
    double mChill;
    double mMagicNumber;
    Scatter(double increase=0.0,double altIncrease=0.0, double chill=0.0, double magicNumber=100);
    void Update(double DeltaTime);
    void Shot(bool alt);
    double GetCalculated();
};

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
    Scatter& GetScatter();
    Weapon( int32_t Id );

    ActorFactory& mActorFactory;
    double mCooldown;
    double mShootCooldown;
    double mShootAltCooldown;
    Scatter mScatter;
    bool mShoot;
    bool mShootAlt;
    friend class ItemFactory;
};

#endif//INCLUDED_CORE_WEAPON_ASSET_H
