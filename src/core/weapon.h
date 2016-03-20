#ifndef INCLUDED_CORE_WEAPON_ASSET_H
#define INCLUDED_CORE_WEAPON_ASSET_H
#include "core/item.h"
#include "core/actor_factory.h"
#include "platform/export.h"

struct Scatter
{
    double mCurrent;
    double mIncrease;
    double mAltIncrease;
    double mChill;
    double mMagicNumber;
    Scatter( double increase = 0.0, double altIncrease = 0.0, double chill = 0.0, double magicNumber = 100 );
    void Update( double DeltaTime, int32_t accuracy );
    void Shot( bool alt );
    double GetCalculated() const;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void Scatter::serialize( Archive& ar, const unsigned int version )
{
    ar& mCurrent;
    ar& mIncrease;
    ar& mAltIncrease;
    ar& mChill;
    ar& mMagicNumber;
}

class Weapon : public Item
{
public:
    virtual bool IsShooting() const;
    bool GetShoot();
    void SetShoot( bool shoot );
    virtual bool IsShootingAlt() const;
    bool GetShootAlt();
    void SetShootAlt( bool shoot );
    double GetCooldown() const;
    void SetCooldown( double cooldown );
    double GetShootCooldown() const;
    void SetShootCooldown( double cooldown );
    double GetShootAltCooldown() const;
    void SetShootAltCooldown( double cooldown );
    Scatter& GetScatter();
    // used by ItemLoader
    void SetScatter( Scatter scatter );
    void SetBullets( double bullets );
    double GetBullets()const;
    void SetBulletsMax( double bulletsMax );
    double GetBulletsMax()const;
    void SetShotCost( int32_t shotCost );
    int32_t GetShotCost()const;
    void SetShotCostAlt( int32_t shotCostAlt );
    int32_t GetShotCostAlt()const;
    void SetReloadTime( double reloadTime );
    double GetReloadTime()const;
    void SetReloadTimeMax( double reloadTimeMax );
    double GetReloadTimeMax()const;
    void SetStaticReload( double staticReload );
    double GetStaticReload()const;
    void SetMuzzleId( int32_t muzzleId);
    double GetMuzzleId()const;
    void SetMuzzleAltId(int32_t muzzleAltId);
    double GetMuzzleAltId()const;
    void SetPositionX(double x);
    double GetPositionX()const;
    void SetPositionY(double y);
    double GetPositionY()const;

    virtual bool CanReload() const;
    virtual glm::vec3 GetMouseColor() const;
    virtual double GetMouseSize() const;
    virtual std::string GetMouseText() const;
    virtual bool IsMouseResizable() const;
    Weapon( int32_t Id );
    Weapon();
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
protected:
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
    int32_t mMuzzleId;
    int32_t mMuzzleAltId;
    double mPositionX;
    double mPositionY;
    friend class ItemFactory;
};

template<class Archive>
void Weapon::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Item>( *this );
    ar& mCooldown;
    ar& mShootCooldown;
    ar& mShootAltCooldown;
    ar& mScatter;
    ar& mShoot;
    ar& mShootAlt;
    ar& mBullets;
    ar& mBulletsMax;
    ar& mShotCost;
    ar& mShotCostAlt;
    ar& mReloadTime;
    ar& mReloadTimeMax;
    ar& mStaticReload;
}

class WeaponLoader: public ItemLoader<Weapon>
{
public:
    virtual void BindValues();
    WeaponLoader();
    friend class ItemLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( Weapon, Weapon, "weapon" );
#endif//INCLUDED_CORE_WEAPON_ASSET_H
