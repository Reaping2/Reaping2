#include "platform/i_platform.h"
#include "core/weapon.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/scene.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

Weapon::Weapon( int32_t Id )
    : Item( Id )
    , mActorFactory( ActorFactory::Get() )
    , mCooldown( 0.0 )
    , mShootCooldown( 1.0 )
    , mShootAltCooldown( 1.0 )
    , mScatter( 0 )
    , mShoot( false )
    , mShootAlt( false )
    , mBullets( 0.0 )
    , mBulletsMax( 10.0 )
    , mShotCost( 1 )
    , mShotCostAlt( 1 )
    , mReloadTime( 0.0 )
    , mReloadTimeMax( 2.0 )
    , mStaticReload( 0.0 )
    , mMuzzleId( -1 )
    , mMuzzleAltId( -1 )
    , mPositionX( 0 )
    , mPositionY( 0 )
{
    mType = ItemType::Weapon;
}

Weapon::Weapon()
    : Item()
    , mActorFactory( ActorFactory::Get() )
    , mCooldown( 0.0 )
    , mShootCooldown( 1.0 )
    , mShootAltCooldown( 1.0 )
    , mScatter( 0 )
    , mShoot( false )
    , mShootAlt( false )
    , mBullets( 0.0 )
    , mBulletsMax( 10.0 )
    , mShotCost( 1 )
    , mShotCostAlt( 1 )
    , mReloadTime( 0.0 )
    , mReloadTimeMax( 2.0 )
    , mStaticReload( 0.0 )
    , mMuzzleId( -1 )
    , mMuzzleAltId( -1 )
    , mPositionX( 0 )
    , mPositionY( 0 )
    , mShotId( -1 )
    , mShotAltId( -1 )
{

}


void Weapon::InitSumBullets( Limited<double> sumBullets )
{
    mSumBullets = sumBullets;
}

bool Weapon::IsShooting() const
{
    return mShoot && mBullets >= mShotCost;
}

void Weapon::SetShoot( bool shoot )
{
    mShoot = shoot;
}

bool Weapon::IsShootingAlt() const
{
    return mShootAlt && mBullets >= mShotCostAlt;
}

void Weapon::SetShootAlt( bool shoot )
{
    mShootAlt = shoot;
}

double Weapon::GetCooldown() const
{
    return mCooldown;
}

void Weapon::SetCooldown( double cooldown )
{
    mCooldown = cooldown;
}

double Weapon::GetShootCooldown() const
{
    return mShootCooldown;
}

void Weapon::SetShootCooldown( double cooldown )
{
    mShootCooldown = cooldown;
}

double Weapon::GetShootAltCooldown() const
{
    return mShootAltCooldown;
}

void Weapon::SetShootAltCooldown( double cooldown )
{
    mShootAltCooldown = cooldown;
}

Scatter& Weapon::GetScatter()
{
    return mScatter;
}

void Weapon::SetBullets( double bullets )
{
    mBullets = bullets;
}

double Weapon::GetBullets()const
{
    return mBullets;
}

void Weapon::SetBulletsMax( double bulletsMax )
{
    mBulletsMax = bulletsMax;
}

double Weapon::GetBulletsMax()const
{
    return mBulletsMax;
}


void Weapon::SetNextReloadBulletCount( double nextReloadBulletCount )
{
    mNextReloadBulletCount = nextReloadBulletCount;
}

double Weapon::GetNextReloadBulletCount() const
{
    return mNextReloadBulletCount;
}

void Weapon::SetShotCost( int32_t shotCost )
{
    mShotCost = shotCost;
}

int32_t Weapon::GetShotCost()const
{
    return mShotCost;
}

void Weapon::SetShotCostAlt( int32_t shotCostAlt )
{
    mShotCostAlt = shotCostAlt;
}

int32_t Weapon::GetShotCostAlt()const
{
    return mShotCostAlt;
}

void Weapon::SetReloadTime( double reloadTime )
{
    mReloadTime = reloadTime;
}

double Weapon::GetReloadTime()const
{
    return mReloadTime;
}

void Weapon::SetReloadTimeMax( double reloadTimeMax )
{
    mReloadTimeMax = reloadTimeMax;
}


void Weapon::Reload()
{
    auto const missingBullets = std::min( (mBulletsMax-mBullets ), mBulletsMax );
    auto const increase = mSumBullets.GetMax()!=0.0?std::min( missingBullets, mSumBullets.Get() ):missingBullets;
    mNextReloadBulletCount = increase + mBullets;
    mSumBullets.Set( mSumBullets.Get() - increase );
    mBullets = 0.0;
    mReloadTime = mReloadTimeMax;
}


void Weapon::StaticReload()
{
    auto const missingBullets = std::min( (mBullets + mStaticReload), mBulletsMax ) - mBullets;
    auto const increase = mSumBullets.GetMax() != 0.0 ? std::min(missingBullets, mSumBullets.Get()):missingBullets;
    mBullets += increase;
    mSumBullets.Set( mSumBullets.Get() - increase );
    mReloadTime = mReloadTimeMax;
}

double Weapon::GetReloadTimeMax()const
{
    return mReloadTimeMax;
}

void Weapon::SetStaticReload( double staticReload )
{
    mStaticReload = staticReload;
}

double Weapon::GetStaticReload()const
{
    return mStaticReload;
}

void Weapon::SetMuzzleId(int32_t muzzleId)
{
    mMuzzleId = muzzleId;
}

double Weapon::GetMuzzleId() const
{
    return mMuzzleId;
}

void Weapon::SetMuzzleAltId(int32_t muzzleAltId)
{
    mMuzzleAltId = muzzleAltId;
}

double Weapon::GetMuzzleAltId() const
{
    return mMuzzleId;
}


void Weapon::SetPositionX(double x)
{
    mPositionX = x;
}


double Weapon::GetPositionX() const
{
    return mPositionX;
}


void Weapon::SetPositionY(double y)
{
    mPositionY = y;
}


double Weapon::GetPositionY() const
{
    return mPositionY;
}


void Weapon::SetShotId( int32_t Id )
{
    mShotId = Id;
}


int32_t Weapon::GetShotId() const
{
    return mShotId;
}


void Weapon::SetShotAltId( int32_t Id )
{
    mShotAltId = Id;
}


int32_t Weapon::GetShotAltId() const
{
    return mShotAltId;
}


bool Weapon::CanSwitch() const
{
    return Item::CanSwitch();
}


void Weapon::Deselected()
{
    mShoot = false;
    mShootAlt = false;
}

void Weapon::Selected()
{
    Item::Selected();
}

bool Weapon::GetShoot()
{
    return mShoot;
}

bool Weapon::GetShootAlt()
{
    return mShootAlt;
}

glm::vec3 Weapon::GetMouseColor() const
{
    return mReloadTime > 0.0 && mStaticReload == 0.0 ?
           glm::vec3( 1.0, 0.0, 0.0 ) : glm::vec3( 0.0, 0.0, 1.0 );
}

double Weapon::GetMouseSize() const
{
    return mReloadTime > 0.0 && mStaticReload == 0.0 ?
           mReloadTime / mReloadTimeMax * 150 : mScatter.GetCalculated() * 300;
}

std::string Weapon::GetMouseText() const
{
    return boost::lexical_cast<std::string>(
               std::max(   //no negative bullets shown. could happen if shooting comes from server earlier then the client finishes reloading
                   0.0, std::floor( mBullets )
               ) );
}

bool Weapon::IsMouseResizable() const
{
    return mReloadTime <= 0.0 || mStaticReload != 0.0;
}

Limited<double>& Weapon::GetSumBullets()
{
    return mSumBullets;
}


Limited<double> const& Weapon::GetSumBullets() const
{
    return mSumBullets;
}

bool Weapon::CanReload() const
{
    return mReloadTime <= 0.0
           && mBullets != mBulletsMax
           && mStaticReload == 0.0
           && (mSumBullets.GetMax()==0.0||mSumBullets.Get()>0.0);
}

void Weapon::SetScatter( Scatter scatter )
{
    mScatter = scatter;
}



Scatter::Scatter( double increase/*=0.0*/, double altIncrease/*=0.0*/, double chill/*=0.0*/, double magicNumber/*=100*/ )
    : mCurrent( 0.0 )
    , mIncrease( increase )
    , mAltIncrease( altIncrease )
    , mChill( chill )
    , mMagicNumber( magicNumber )
{

}

void Scatter::Update( double DeltaTime, int32_t accuracy )
{
    //want to chill the scatter on a linear way
    // e.g. magic=100, current=100, calculated=1/2,
    // newCalc=50-chill*dt;
    // chill=20, dt=1.0; // lets say 1 sec with 20 chill means newCalc is 20
    double newCalculated = ( GetCalculated() * mMagicNumber - ( mChill * ( 600 + accuracy ) / 600 ) * DeltaTime ) / mMagicNumber;
    newCalculated = std::max( 0.0, newCalculated );
    //    L1("newCalc: %f calc: %f \n",newCalculated,mCurrent);
    if( newCalculated != 0.0 )
    {
        mCurrent = ( newCalculated * mMagicNumber ) / ( 1 - newCalculated ); //inverse of GetCalculated
    }
    //    L1("newCurrent: %f \n",mCurrent);
}

void Scatter::Shot( bool alt )
{
    mCurrent += alt ? mAltIncrease : mIncrease;
}

double Scatter::GetCalculated() const
{
    return mCurrent / ( mMagicNumber + mCurrent );
}

void WeaponLoader::BindValues()
{
    L2( "Bind Weapon values \n" );
    Bind( "shoot_cooldown", func_double( &Weapon::SetShootCooldown ) );
    Bind( "shoot_alt_cooldown", func_double( &Weapon::SetShootAltCooldown ) );
    Bind( "bullets", func_double( &Weapon::SetBullets ) );
    Bind( "bullets", func_double( &Weapon::SetBulletsMax ) );
    Bind( "shot_cost", func_int32_t( &Weapon::SetShotCost ) );
    Bind( "shot_cost_alt", func_int32_t( &Weapon::SetShotCostAlt ) );
    Bind( "reload_time", func_double( &Weapon::SetReloadTimeMax ) );
    Bind( "static_reload", func_double( &Weapon::SetStaticReload ) );
    Bind( "position_x", func_double(&Weapon::SetPositionX));
    Bind( "position_y", func_double(&Weapon::SetPositionY));
    std::string istr;
    if (Json::GetStr((*mSetters)["muzzle"], istr))
    {
        Bind<int32_t>(&Weapon::SetMuzzleId, AutoId(istr));
    }
    if (Json::GetStr((*mSetters)["muzzle_alt"], istr))
    {
        Bind<int32_t>(&Weapon::SetMuzzleAltId, AutoId(istr));
    }

    Scatter scatter;
    Json::GetDouble( ( *mSetters )["scatter_increase"], scatter.mIncrease );
    Json::GetDouble( ( *mSetters )["scatter_alt_increase"], scatter.mAltIncrease );
    Json::GetDouble( ( *mSetters )["scatter_chill"], scatter.mChill );
    Json::GetDouble( ( *mSetters )["scatter_magic_number"], scatter.mMagicNumber );
    L2( "Scatter in: %f %f %f %f", scatter.mIncrease, scatter.mAltIncrease, scatter.mChill, scatter.mMagicNumber );
    Bind<Scatter>( &Weapon::SetScatter, scatter );
    if (Json::GetStr( (*mSetters)["shot"], istr ))
    {
        Bind<int32_t>( &Weapon::SetShotId, AutoId( istr ) );
    }
    if (Json::GetStr( (*mSetters)["shot_alt"], istr ))
    {
        Bind<int32_t>( &Weapon::SetShotAltId, AutoId( istr ) );
    }
    Limited<double> sumBullets;
    sumBullets.Load( (*mSetters)["sum_bullets"] );
    Bind<Limited<double>>(&Weapon::InitSumBullets, sumBullets);
}

WeaponLoader::WeaponLoader()
{
}
REAPING2_CLASS_EXPORT_IMPLEMENT( Weapon, Weapon );
