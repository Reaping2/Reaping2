#include "engine/items/shotgun_weapon_sub_system.h"

namespace engine {

ShotgunWeaponSubSystem::ShotgunWeaponSubSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
    , mWeaponItemSubSystem(WeaponItemSubSystem::Get())
    , mActorFactory(ActorFactory::Get())
    , mShotId( AutoId( "shotgun_projectile" ) )
    , mShotAltId( AutoId( "shotgun_alt_projectile" ) )
{
}

void ShotgunWeaponSubSystem::Init()
{
    SubSystemHolder::Init();
}

void ShotgunWeaponSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<Weapon> weapon = inventoryC->GetSelectedWeapon();
    if (weapon->GetCooldown()>0)
    {
        return;
    }
    if (weapon->IsShoot())
    {
        EventServer<AudibleEvent>::Get().SendEvent( AudibleEvent( mShotId ) );

        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps;

        for (int i=0;i<7;++i)
        {
            ps = mActorFactory(mShotId);
            ps->Get<IPositionComponent>()->SetOrientation( -0.4 + i*0.13333 );
            projectiles.push_back( Opt<Actor>(ps.release()) );
        }

        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetScatter(),false);
    }
    else if (weapon->IsShootAlt())
    {
        EventServer<AudibleEvent>::Get().SendEvent( AudibleEvent( mShotId ) );

        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory(mShotAltId);
        projectiles.push_back( Opt<Actor>(ps.release()) );


        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetScatter(),true);
    }
}

} // namespace engine

