#include "engine/items/shotgun_weapon_sub_system.h"
#include "core/i_audible_component.h"
#include "core/i_fade_out_component.h"
#include "core/i_move_component.h"
#include "../explode_on_death_system.h"
#include "core/shotgun.h"

namespace engine {

ShotgunWeaponSubSystem::ShotgunWeaponSubSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
    , mWeaponItemSubSystem( WeaponItemSubSystem::Get() )
    , mActorFactory( ActorFactory::Get() )
{
}

void ShotgunWeaponSubSystem::Init()
{
    SubSystemHolder::Init();
}

void ShotgunWeaponSubSystem::Update( Actor& actor, double DeltaTime )
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<Weapon> weapon = inventoryC->GetSelectedItem( ItemType::Weapon );
    if ( weapon->GetCooldown() > 0 )
    {
        return;
    }
    Opt<IAudibleComponent> ac = actor.Get<IAudibleComponent>();
    if ( weapon->IsShooting() )
    {
        WeaponItemSubSystem::Projectiles_t projectiles;

        Opt<Shotgun> shotgun( weapon );
        if( shotgun.IsValid() )
        {
            WeaponItemSubSystem::Projectiles_t projectiles;
            ExplodeOnDeathSystem::FillExplosionProjectiles( *shotgun.Get(), actor, projectiles );
            mWeaponItemSubSystem->AddProjectiles( actor, projectiles, weapon->GetScatter(), false );
        }
        if( ac.IsValid() )
        {
            ac->AddOneShotEffect( weapon->GetShotId() );
        }
    }
    else if ( weapon->IsShootingAlt() )
    {
        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory( weapon->GetShotAltId() );
        projectiles.push_back( Opt<Actor>( ps.release() ) );


        mWeaponItemSubSystem->AddProjectiles( actor, projectiles, weapon->GetScatter(), true );
        if( ac.IsValid() )
        {
            ac->AddOneShotEffect( weapon->GetShotAltId() );
        }
    }
}

} // namespace engine

