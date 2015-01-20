#include "platform/i_platform.h"
#include "engine/items/plasma_gun_weapon_sub_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "core/i_inventory_component.h"
#include "core/weapon.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "inventory_system.h"
#include "core/audible_event.h"

namespace engine {

PlasmaGunWeaponSubSystem::PlasmaGunWeaponSubSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
    , mWeaponItemSubSystem(WeaponItemSubSystem::Get())
    , mActorFactory(ActorFactory::Get())
    , mShotId( AutoId( "plasma" ) )
    , mAltShotId( AutoId( "plasma_alt" ) )
    , mPlasmaShotId( AutoId( "plasma_shot" ) )
{
}

void PlasmaGunWeaponSubSystem::Init()
{
    SubSystemHolder::Init();
}

void PlasmaGunWeaponSubSystem::Update(Actor& actor, double DeltaTime)
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

        std::auto_ptr<Actor> ps = mActorFactory(mPlasmaShotId);
        ps->AddAction( AutoId( "move" ) );
        projectiles.push_back( Opt<Actor>(ps.release()) );

        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetScatter());
    }
    else if (weapon->IsShootAlt())
    {
        EventServer<AudibleEvent>::Get().SendEvent( AudibleEvent( mAltShotId ) );

        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory(mPlasmaShotId);
        ps->AddAction( AutoId( "move" ) );
        ps->Get<IPositionComponent>()->SetOrientation( -0.15 );
        projectiles.push_back( Opt<Actor>(ps.release()) );

        ps = mActorFactory(mPlasmaShotId);
        ps->AddAction( AutoId( "move" ) );
        ps->Get<IPositionComponent>()->SetOrientation( 0.15 );
        projectiles.push_back( Opt<Actor>(ps.release()) );

        ps = mActorFactory(mPlasmaShotId);
        ps->AddAction( AutoId( "move" ) );
        projectiles.push_back( Opt<Actor>(ps.release()) );

        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetAltScatter());
    }
}

} // namespace engine

