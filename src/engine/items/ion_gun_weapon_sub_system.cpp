#include "engine/items/ion_gun_weapon_sub_system.h"

namespace engine {

IonGunWeaponSubSystem::IonGunWeaponSubSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
    , mWeaponItemSubSystem(WeaponItemSubSystem::Get())
    , mActorFactory(ActorFactory::Get())
    , mShotId( AutoId( "ion_gun_projectile" ) )
    , mAltShotId( AutoId( "ion_gun_alt_projectile" ) )
{
}

void IonGunWeaponSubSystem::Init()
{
    SubSystemHolder::Init();
}

void IonGunWeaponSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<Weapon> weapon = inventoryC->GetSelectedWeapon();
    if (weapon->GetCooldown()>0)
    {
        return;
    }
    if (weapon->IsShooting())
    {
        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory(mShotId);
        projectiles.push_back( Opt<Actor>(ps.release()) );

        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetScatter(),false);
    }
    else if (weapon->IsShootingAlt())
    {
        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory(mAltShotId);
        projectiles.push_back( Opt<Actor>(ps.release()) );


        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetScatter(),true);
    }
}

} // namespace engine

