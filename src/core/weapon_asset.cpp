#include "i_core.h"

WeaponAsset::WeaponAsset( int32_t Id )
    : Item( Id )
    , mCooldown( 0.0 )
    , mShootCooldown( 1.0 )
    , mShootAltCooldown( 1.0 )
    , mScatter( 0 )
    , mAltScatter( 0 )
{
    mType = Weapon;
}
void WeaponAsset::Update( double Seconds )
{
    Item::Update( Seconds );
    double cd = mCooldown;
    cd -= Seconds;
    if( cd < 0 )
    {
        cd = 0;
    }
    mCooldown = cd;
    if( !mActor )
    {
        return;
    }
    static const int32_t ShootId = AutoId( "shoot" );
    static const int32_t ShootAltId = AutoId( "shoot_alt" );
    if( mActor->HasAction( ShootId ) )
    {
        Shoot();
    }
    if( mActor->HasAction( ShootAltId ) )
    {
        ShootAlt();
    }
}

void WeaponAsset::Shoot()
{
    if( !mActor )
    {
        return;
    }
    if( mCooldown != 0.0 )
    {
        return;
    }

    Projectiles_t Projectiles;
    ShootImpl( Projectiles );
    Scene& Scen( Scene::Get() );
    double ori = mActor->GetOrientation();
    if( mScatter )
    {
        ori += ( rand() % mScatter - mScatter / 2. ) * 0.01 * boost::math::double_constants::pi;
    }
    for( Projectiles_t::iterator i = Projectiles.begin(), e = Projectiles.end(); i != e; ++i )
    {
        Shot& Proj = *i;
		PositionComponent& projPositionC = Proj.GetComponent<PositionComponent>( AutoId("position_component") );
		PositionComponent& actorPositionC = mActor->GetComponent<PositionComponent>( AutoId("position_component") );
		projPositionC.SetX( actorPositionC.GetX() );
		projPositionC.SetY( actorPositionC.GetY() );
        Proj.SetParent( *mActor );
        Proj.SetOrientation( Proj.GetOrientation() + ori );
        Proj.SetHeading( Proj.GetOrientation() );
        Scen.AddActor( &Proj );
    }
    Projectiles.release().release();

    mCooldown = mShootCooldown;
}

void WeaponAsset::ShootAlt()
{
    if( !mActor )
    {
        return;
    }
    if( mCooldown != 0.0 )
    {
        return;
    }

    Projectiles_t Projectiles;
    ShootAltImpl( Projectiles );
    Scene& Scen( Scene::Get() );
    double ori = mActor->GetOrientation();
    if( mAltScatter )
    {
        ori += ( rand() % mAltScatter - mAltScatter / 2. ) * 0.01 * boost::math::double_constants::pi;
    }
    for( Projectiles_t::iterator i = Projectiles.begin(), e = Projectiles.end(); i != e; ++i )
    {
        Shot& Proj = *i;
		PositionComponent& projPositionC = Proj.GetComponent<PositionComponent>( AutoId("position_component") );
		PositionComponent& actorPositionC = mActor->GetComponent<PositionComponent>( AutoId("position_component") );
		projPositionC.SetX( actorPositionC.GetX() );
		projPositionC.SetY( actorPositionC.GetY() );
        Proj.SetParent( *mActor );
        Proj.SetOrientation( Proj.GetOrientation() + ori );
        Proj.SetHeading( Proj.GetOrientation() );
        Scen.AddActor( &Proj );
    }
    Projectiles.release().release();

    mCooldown = mShootAltCooldown;
}
