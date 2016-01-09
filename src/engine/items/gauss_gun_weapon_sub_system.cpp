#include "engine/items/gauss_gun_weapon_sub_system.h"
#include "core/gauss_gun.h"

namespace engine {

GaussGunWeaponSubSystem::GaussGunWeaponSubSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
    , mWeaponItemSubSystem(WeaponItemSubSystem::Get())
    , mActorFactory(ActorFactory::Get())
    , mShotId( AutoId( "gauss_shot" ) )
    , mAltShotId( AutoId( "gauss_alt_shot" ) )
{
}

void GaussGunWeaponSubSystem::Init()
{
    SubSystemHolder::Init();
}

void GaussGunWeaponSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<GaussGun> weapon = inventoryC->GetSelectedWeapon();
    Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
    if ( weapon->GetShootAlt() )
    {
        weapon->StartCharge();
    }
    else
    {
        weapon->EndCharge();
    }
    if ( weapon->GetCooldown() )
    {
        weapon->EndCharge();
    }
    if (moveC.IsValid())
    {
        moveC->SetRooted( weapon->IsCharging() );
    }
    if (weapon->GetCooldown()>0)
    {
        return;
    }
    if (weapon->IsShooting())
    {
        EventServer<AudibleEvent>::Get().SendEvent( AudibleEvent( mShotId ) );

        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory(mShotId);
        projectiles.push_back( Opt<Actor>(ps.release()) );

        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetScatter(),false);
    }
    else if (weapon->IsShootingAlt())
    {
        EventServer<AudibleEvent>::Get().SendEvent( AudibleEvent( mShotId ) );

        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory(mAltShotId);
        projectiles.push_back( Opt<Actor>(ps.release()) );

        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetScatter(),true);
    }
}

} // namespace engine

