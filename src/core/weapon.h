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
    double GetCalculated() const;
};

class Weapon : public Item
{
public:
    virtual bool IsShooting() const;
    bool GetShoot();
    void SetShoot(bool shoot);
    virtual bool IsShootingAlt() const;
    bool GetShootAlt();
    void SetShootAlt(bool shoot);
    double GetCooldown() const;
    void SetCooldown(double cooldown);
    double GetShootCooldown() const;
    void SetShootCooldown(double cooldown);
    double GetShootAltCooldown() const;
    void SetShootAltCooldown(double cooldown);
    Scatter& GetScatter();
    void SetBullets(double bullets);
    double GetBullets()const;
    void SetBulletsMax(double bulletsMax);
    double GetBulletsMax()const;
    void SetShotCost(int32_t shotCost);
    int32_t GetShotCost()const;
    void SetShotCostAlt(int32_t shotCostAlt);
    int32_t GetShotCostAlt()const;
    void SetReloadTime(double reloadTime);
    double GetReloadTime()const;
    void SetReloadTimeMax(double reloadTimeMax);
    double GetReloadTimeMax()const;
    void SetStaticReload(double staticReload);
    double GetStaticReload()const;

    virtual glm::vec3 GetMouseColor() const;
    virtual double GetMouseSize() const;
    virtual std::string GetMouseText() const;
    Weapon( int32_t Id );

    ActorFactory& mActorFactory;
    double mCooldown;
    double mShootCooldown;
    double mShootAltCooldown;
    Scatter mScatter;
    bool mShoot;
    bool mShootAlt;
    double mBullets;
    double mBulletsMax;
    int32_t mShotCost;
    int32_t mShotCostAlt;
    double mReloadTime;
    double mReloadTimeMax;
    double mStaticReload;
    friend class ItemFactory;
};

#endif//INCLUDED_CORE_WEAPON_ASSET_H
