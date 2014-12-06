#include "platform/i_platform.h"
#include "core/pistol.h"
#include "core/audible_event.h"
#include "core/i_move_component.h"

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
    std::auto_ptr<Actor> ps(mActorFactory(mShotId));
    ps->AddAction( AutoId( "move" ) );
    Projectiles.push_back( ps );
}

void Pistol::ShootAltImpl( Projectiles_t& Projectiles )
{
    EventServer<AudibleEvent>::Get().SendEvent( AudibleEvent( mShotId ) );
    std::auto_ptr<Actor> ps(mActorFactory(mShotId));
    ps->AddAction( AutoId( "move" ) );
    Projectiles.push_back( ps );
}
