#include "engine/items/rusty_reaper_weapon_sub_system.h"
#include "core/rusty_reaper.h"
#include "core/i_attachable_component.h"
#include "core/shot_collision_component.h"
#include "core/i_rotate_component.h"
#include "core/item_dropped_event.h"
#include "core/i_health_component.h"

namespace engine {

RustyReaperWeaponSubSystem::RustyReaperWeaponSubSystem()
    : mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
    , mWeaponItemSubSystem( WeaponItemSubSystem::Get() )
    , mActorFactory( ActorFactory::Get() )
    , mProjectileId( AutoId( "rusty_reaper_projectile" ) )
    , mProjectileAltId( AutoId( "rusty_reaper_alt_projectile" ) )
    , mWeaponId( AutoId( "rusty_reaper" ) )
{
}


void RustyReaperWeaponSubSystem::Init()
{
    mOnItemDropped = EventServer<core::ItemDroppedEvent>::Get().Subscribe( boost::bind( &RustyReaperWeaponSubSystem::OnItemDropped, this, _1 ) );
}


void RustyReaperWeaponSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<RustyReaper> weapon = inventoryC->GetSelectedWeapon();
    if (weapon->GetSawGUID() == -1)
    {
        WeaponItemSubSystem::Projectiles_t projectiles;
        std::auto_ptr<Actor> ps = mActorFactory( mProjectileAltId );
        Opt<ctf::IAttachableComponent> attachableC( ps->Get<ctf::IAttachableComponent>() );
        BOOST_ASSERT( attachableC.IsValid() );
        attachableC->SetAttachedGUID( actor.GetGUID() );
        weapon->SetSawGUID( ps->GetGUID() );
        projectiles.push_back( Opt<Actor>( ps.release() ) );
        mWeaponItemSubSystem->AddProjectiles( actor, projectiles, weapon->GetScatter(), true );
    }
    if (weapon->GetCooldown() > 0)
    {
        return;
    }
    Opt<Actor> saw( mScene.GetActor( weapon->GetSawGUID() ) );
    BOOST_ASSERT( saw.IsValid() );
    Opt<ShotCollisionComponent> sawShotCC( saw->Get<ShotCollisionComponent>() );
    BOOST_ASSERT( sawShotCC.IsValid() );
    sawShotCC->SetDoDamage( false );
    sawShotCC->ResetDamagedActorIds();
    Opt<IRotateComponent> sawRotateC( saw->Get<IRotateComponent>() );
    BOOST_ASSERT( sawRotateC.IsValid() );
    sawRotateC->SetRotating( false );
    if (weapon->IsShooting())
    {
        WeaponItemSubSystem::Projectiles_t projectiles;
        std::auto_ptr<Actor> ps = mActorFactory( mProjectileId );
        projectiles.push_back( Opt<Actor>( ps.release() ) );
        mWeaponItemSubSystem->AddProjectiles( actor, projectiles, weapon->GetScatter(), false );
    }
    else if ( weapon->IsShootingAlt() )
    {
        sawShotCC->SetDoDamage( true );
        sawRotateC->SetRotating( true );
    }
}


void RustyReaperWeaponSubSystem::OnItemDropped( core::ItemDroppedEvent const& Evt )
{
    if (Evt.mItem.GetId() == mWeaponId)
    {
        RustyReaper& weapon=static_cast<RustyReaper&>(Evt.mItem);
        Opt<Actor> saw( mScene.GetActor( weapon.GetSawGUID() ) );
        if ( saw.IsValid() )
        {
            Opt<IHealthComponent> healthC( saw->Get<IHealthComponent>() );
            if (healthC.IsValid())
            {
                healthC->SetHP( 0 );
            }
        }
    }
}

} // namespace engine

