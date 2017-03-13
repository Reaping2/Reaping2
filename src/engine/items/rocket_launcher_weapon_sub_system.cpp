#include "engine/items/rocket_launcher_weapon_sub_system.h"

namespace engine {

RocketLauncherWeaponSubSystem::RocketLauncherWeaponSubSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
    , mWeaponItemSubSystem( WeaponItemSubSystem::Get() )
    , mActorFactory( ActorFactory::Get() )
{
}

void RocketLauncherWeaponSubSystem::Init()
{
    SubSystemHolder::Init();
}

void RocketLauncherWeaponSubSystem::Update( Actor& actor, double DeltaTime )
{
    Opt<Weapon> weapon = actor.Get<IInventoryComponent>()->GetSelectedItem( ItemType::Weapon );
    if( !weapon->IsShooting() && !weapon->IsShootingAlt() )
    {
        // not firing, nothing to do
        return;
    }
    if( weapon->GetCooldown() > 0 )
    {
        return;
    }

    WeaponItemSubSystem::Projectiles_t projectiles;

    if( weapon->IsShooting() )
    {
        std::auto_ptr<Actor> rocket = mActorFactory( weapon->GetShotId() );
        projectiles.push_back( rocket.release() );
    }
    else if( weapon->IsShootingAlt() )
    {
        std::auto_ptr<Actor> rocket = mActorFactory( weapon->GetShotAltId() );
        projectiles.push_back( rocket.release() );
    }

    mWeaponItemSubSystem->AddProjectiles( actor, projectiles, weapon->GetScatter(), weapon->IsShootingAlt() );
}

} // namespace engine
