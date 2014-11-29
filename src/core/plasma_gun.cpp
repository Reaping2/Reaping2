#include "platform/i_platform.h"
#include "core/plasma_gun.h"
#include "core/plasma_shot.h"
#include "core/audible_event.h"
#include "core/i_position_component.h"

PlasmaGun::PlasmaGun( int32_t Id )
    : Weapon( Id )
    // todo: move to Weapon base, init from data file
    , mShotId( AutoId( "plasma" ) )
    , mAltShotId( AutoId( "plasma_alt" ) )
{
    mScatter = 10;
    mAltScatter = 3;
    mShootCooldown = 0.1;
    mShootAltCooldown = 0.3;
}

void PlasmaGun::ShootImpl( Projectiles_t& Projectiles )
{
    EventServer<AudibleEvent>::Get().SendEvent( AudibleEvent( mShotId ) );
    Shot* ps = new PlasmaShot();
    Projectiles.push_back( ps );
}

void PlasmaGun::ShootAltImpl( Projectiles_t& Projectiles )
{
    EventServer<AudibleEvent>::Get().SendEvent( AudibleEvent( mAltShotId ) );

    Shot* ps = new PlasmaShot();
    ps->Get<IPositionComponent>()->SetOrientation( -0.15 );
    Projectiles.push_back( ps );

    ps = new PlasmaShot();
    ps->Get<IPositionComponent>()->SetOrientation( 0.15 );
    Projectiles.push_back( ps );

    ps = new PlasmaShot();
    Projectiles.push_back( ps );
}