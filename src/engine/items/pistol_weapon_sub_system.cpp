#include "engine/items/pistol_weapon_sub_system.h"

namespace engine {

PistolWeaponSubSystem::PistolWeaponSubSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
    , mWeaponItemSubSystem( WeaponItemSubSystem::Get() )
    , mActorFactory( ActorFactory::Get() )
{
}

void PistolWeaponSubSystem::Init()
{
    SubSystemHolder::Init();
}

void PistolWeaponSubSystem::Update( Actor& actor, double DeltaTime )
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<Weapon> weapon = inventoryC->GetSelectedItem( ItemType::Weapon );
    if ( weapon->GetCooldown() > 0 )
    {
        return;
    }
    if ( weapon->IsShooting() )
    {
        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory( weapon->GetShotId() );
        projectiles.push_back( Opt<Actor>( ps.release() ) );

        mWeaponItemSubSystem->AddProjectiles( actor, projectiles, weapon->GetScatter(), false );
    }
    else if ( weapon->IsShootingAlt() )
    {
        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory( weapon->GetShotAltId() );
        projectiles.push_back( Opt<Actor>( ps.release() ) );

        mWeaponItemSubSystem->AddProjectiles( actor, projectiles, weapon->GetScatter(), true );
    }
}

} // namespace engine

