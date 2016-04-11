#include "engine/items/rusty_reaper_weapon_sub_system.h"
#include "core/rusty_reaper.h"

namespace engine {

RustyReaperWeaponSubSystem::RustyReaperWeaponSubSystem()
    : mScene(Scene::Get())
    , mProgramState(core::ProgramState::Get())
    , mWeaponItemSubSystem(WeaponItemSubSystem::Get())
    , mActorFactory(ActorFactory::Get())
    , mProjectileId(AutoId("rusty_reaper_projectile"))
    , mProjectileAltId(AutoId("rusty_reaper_alt_projectile"))
{
}


void RustyReaperWeaponSubSystem::Init()
{
}


void RustyReaperWeaponSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<RustyReaper> weapon = inventoryC->GetSelectedWeapon();
    if (weapon->GetCooldown() > 0)
    {
        return;
    }
    if (weapon->IsShooting())
    {
        WeaponItemSubSystem::Projectiles_t projectiles;
        std::auto_ptr<Actor> ps = mActorFactory( mProjectileId );
        projectiles.push_back( Opt<Actor>( ps.release() ) );
        mWeaponItemSubSystem->AddProjectiles( actor, projectiles, weapon->GetScatter(), false );
    }
    else if ( weapon->IsShootingAlt() )
    {
        WeaponItemSubSystem::Projectiles_t projectiles;
        std::auto_ptr<Actor> ps = mActorFactory( mProjectileAltId );
        projectiles.push_back( Opt<Actor>( ps.release() ) );
        mWeaponItemSubSystem->AddProjectiles( actor, projectiles, weapon->GetScatter(), true );
    }
}

} // namespace engine

