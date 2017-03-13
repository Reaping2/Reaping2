#ifndef INCLUDED_CORE_WEAPON_ASSET_H
#define INCLUDED_CORE_WEAPON_ASSET_H
#include "core/item.h"
#include "core/actor_factory.h"
#include "platform/export.h"
#include "buffable.h"

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
    void SetNextReloadBulletCount( double nextReloadBulletCount );
    double GetNextReloadBulletCount() const;
    void SetShotCost( int32_t shotCost );
    int32_t GetShotCost()const;
    void SetShotCostAlt( int32_t shotCostAlt );
    int32_t GetShotCostAlt()const;
    void SetReloadTime( double reloadTime );
    double GetReloadTime()const;
    void SetReloadTimeMax( double reloadTimeMax );
    void Reload();
    void StaticReload();
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
    void SetShotId( int32_t Id );
    int32_t GetShotId()const;
    void SetShotAltId( int32_t Id );
    int32_t GetShotAltId()const;

    virtual bool CanSwitch() const;
    virtual void Deselected();
    virtual void Selected();
    virtual bool CanReload() const;
    virtual glm::vec3 GetMouseColor() const;
    virtual double GetMouseSize() const;
    virtual std::string GetMouseText() const;
    virtual bool IsMouseResizable() const;
    Limited<double> const& GetSumBullets() const;
    Limited<double>& GetSumBullets();
    Weapon( int32_t Id );
    Weapon();
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
protected:
    void InitSumBullets( Limited<double> sumBullets );
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
    int32_t mShotId;
    int32_t mShotAltId;
    Limited<double> mSumBullets;
    double mNextReloadBulletCount = 0.0;
    friend class ItemFactory;
    friend class WeaponLoader;
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
    ar& mShotId;
    ar& mShotAltId;
    ar& mSumBullets;
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
