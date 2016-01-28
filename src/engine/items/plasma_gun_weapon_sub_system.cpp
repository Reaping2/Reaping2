#include "engine/items/plasma_gun_weapon_sub_system.h"
#include "core/i_audible_component.h"

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
    Opt<IAudibleComponent> ac = actor.Get<IAudibleComponent>();
    if (weapon->IsShooting())
    {
        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory(mPlasmaShotId);
        projectiles.push_back( Opt<Actor>(ps.release()) );

        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetScatter(),false);
        if( ac.IsValid() )
        {
            ac->AddOneShotEffect( mShotId );
        }
    }
    else if (weapon->IsShootingAlt())
    {
        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory(mPlasmaShotId);
        ps->Get<IPositionComponent>()->SetOrientation( -0.25 );
        projectiles.push_back( Opt<Actor>(ps.release()) );

        ps = mActorFactory(mPlasmaShotId);
        ps->Get<IPositionComponent>()->SetOrientation( 0.25 );
        projectiles.push_back( Opt<Actor>(ps.release()) );

        ps = mActorFactory(mPlasmaShotId);
        projectiles.push_back( Opt<Actor>(ps.release()) );

        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,weapon->GetScatter(),true);
        if( ac.IsValid() )
        {
            ac->AddOneShotEffect( mAltShotId );
        }
    }
}

} // namespace engine

