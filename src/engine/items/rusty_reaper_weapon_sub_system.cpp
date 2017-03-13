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
    , mWeaponId( AutoId( "rusty_reaper" ) )
{
}


void RustyReaperWeaponSubSystem::Init()
{
    mOnItemChanged = EventServer<engine::ItemChangedEvent>::Get().Subscribe( boost::bind( &RustyReaperWeaponSubSystem::OnItemChanged, this, _1 ) );
}


void RustyReaperWeaponSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<RustyReaper> weapon = inventoryC->GetSelectedItem( ItemType::Weapon );
    if (weapon->GetCooldown() > 0)
    {
        return;
    }
    Opt<Actor> saw( mScene.GetActor( weapon->GetSawGUID() ) );
    if (!saw.IsValid())
    {
        WeaponItemSubSystem::Projectiles_t projectiles;
        std::auto_ptr<Actor> ps = mActorFactory( weapon->GetShotAltId() );
        Opt<ctf::IAttachableComponent> attachableC( ps->Get<ctf::IAttachableComponent>() );
        BOOST_ASSERT( attachableC.IsValid() );
        attachableC->SetAttachedGUID( actor.GetGUID() );
        weapon->SetSawGUID( ps->GetGUID() );
        projectiles.push_back( Opt<Actor>( ps.release() ) );
        mWeaponItemSubSystem->AddProjectiles( actor, projectiles, weapon->GetScatter(), true );
        saw = mScene.GetActor( weapon->GetSawGUID() );
    }
    BOOST_ASSERT( saw.IsValid() );
    Opt<ShotCollisionComponent> sawShotCC( saw->Get<ICollisionComponent>() );
    BOOST_ASSERT( sawShotCC.IsValid() );
    sawShotCC->SetDoDamage( false );
    sawShotCC->ResetDamagedActorIds();
    Opt<IRotateComponent> sawRotateC( saw->Get<IRotateComponent>() );
    BOOST_ASSERT( sawRotateC.IsValid() );
    sawRotateC->SetRotating( false );
    if (weapon->IsShooting())
    {
        WeaponItemSubSystem::Projectiles_t projectiles;
        std::auto_ptr<Actor> ps = mActorFactory( weapon->GetShotId() );
        projectiles.push_back( Opt<Actor>( ps.release() ) );
        mWeaponItemSubSystem->AddProjectiles( actor, projectiles, weapon->GetScatter(), false );
    }
    else if ( weapon->IsShootingAlt() )
    {
        sawShotCC->SetDoDamage( true );
        sawRotateC->SetRotating( true );
    }
}


void RustyReaperWeaponSubSystem::OnItemChanged( engine::ItemChangedEvent const& Evt )
{
    if (Evt.mPrevItemId == mWeaponId)
    {
        auto actor(mScene.GetActor( Evt.mActorGUID ));
        if (!actor.IsValid())
        {
            return;
        }
        Opt<IInventoryComponent> inventoryC = actor->Get<IInventoryComponent>();
        if (!inventoryC.IsValid())
        {
            return;
        }
        Opt<RustyReaper> weapon = inventoryC->GetItem( Evt.mPrevItemId );
        if (!weapon.IsValid())
        {
            return;
        }
        Opt<Actor> saw( mScene.GetActor( weapon->GetSawGUID() ) );
        if (saw.IsValid())
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

