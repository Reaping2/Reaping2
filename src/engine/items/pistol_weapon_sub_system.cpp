#include "engine/items/pistol_weapon_sub_system.h"

namespace engine {

PistolWeaponSubSystem::PistolWeaponSubSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
    , mWeaponItemSubSystem(WeaponItemSubSystem::Get())
    , mActorFactory(ActorFactory::Get())
    , mShotId( AutoId( "pistol_shot" ) )
{
}

void PistolWeaponSubSystem::Init()
{
    SubSystemHolder::Init();
}

void PistolWeaponSubSystem::Update(Actor& actor, double DeltaTime)
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

        std::auto_ptr<Actor> ps = mActorFactory(mShotId);
        projectiles.push_back( Opt<Actor>(ps.release()) );

        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetScatter(),false);
    }
    else if (weapon->IsShootAlt())
    {
        EventServer<AudibleEvent>::Get().SendEvent( AudibleEvent( mShotId ) );

        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory(mShotId);
        projectiles.push_back( Opt<Actor>(ps.release()) );

        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetScatter(),true);
    }
}

} // namespace engine

