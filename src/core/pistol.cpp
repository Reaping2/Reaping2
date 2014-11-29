#include "i_core.h"
#include "core/pistol.h"
#include "core/pistol_shot.h"
#include "core/audible_event.h"

Pistol::Pistol( int32_t Id )
    : Weapon( Id )
    // todo: move to Weapon base, init from data file
    , mShotId( AutoId( "pistol_shot" ) )
{
    mShootCooldown = 0.2;
    mShootAltCooldown = 0.1;
    mScatter = 10;
    mAltScatter = 30;
}

void Pistol::ShootImpl( Projectiles_t& Projectiles )
{
    EventServer<AudibleEvent>::Get().SendEvent( AudibleEvent( mShotId ) );
    Projectiles.push_back( new PistolShot() );
}

void Pistol::ShootAltImpl( Projectiles_t& Projectiles )
{
    EventServer<AudibleEvent>::Get().SendEvent( AudibleEvent( mShotId ) );
    Projectiles.push_back( new PistolShot() );
}
