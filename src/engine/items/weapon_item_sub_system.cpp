#include "platform/i_platform.h"
#include "engine/items/weapon_item_sub_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "core/i_inventory_component.h"
#include "core/weapon.h"
#include "core/i_position_component.h"
#include "core/shot_collision_component.h"
#include "core/i_move_component.h"
#include "inventory_system.h"
#include "core/i_accuracy_component.h"
#include "platform/event.h"
#include "core/shot_event.h"
#include "core/i_owner_component.h"
#include "../item_properties_changed_event.h"
#include "../system_suppressor.h"

namespace engine {

WeaponItemSubSystem::WeaponItemSubSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
    , mActorFactory( ActorFactory::Get() )
{

}

void WeaponItemSubSystem::Init()
{
    SystemSuppressor::Get().Add( SystemSuppressor::SceneLoad, GetType_static() );

    SubSystemHolder::Init();
    mOnShot = EventServer<core::ShotEvent>::Get().Subscribe( boost::bind( &WeaponItemSubSystem::OnShot, this, _1 ) );
}

void WeaponItemSubSystem::Update( Actor& actor, double DeltaTime )
{
    bool dirty = false;
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<Weapon> weapon = inventoryC->GetSelectedItem( ItemType::Weapon );
    if ( !weapon.IsValid() )
    {
        return;
    }
    double cd = weapon->GetCooldown();
    cd -= DeltaTime;
    if( cd < 0 )
    {
        cd = 0;
    }
    weapon->SetCooldown( cd );
    //scatter updated on client
    Opt<IAccuracyComponent> accuracyC = actor.Get<IAccuracyComponent>();
    weapon->GetScatter().Update( DeltaTime, accuracyC.IsValid() ? accuracyC->GetAccuracy().Get() : 0 );

    if (mProgramState.mMode != core::ProgramState::Client)
    {
        if (weapon->CanReload()
            //Not enough bullet for current shot  Need to see if the player wants to shoot alt:
            && (weapon->GetShoot() && weapon->GetBullets() < weapon->GetShotCost()
                && weapon->GetCooldown() <= 0
                //Not enough bullet for current alt shot. Need to see if the player wants to shoot normal:
                || weapon->GetShootAlt() && weapon->GetBullets() < weapon->GetShotCostAlt()
                && weapon->GetCooldown() <= 0
                //Not enough bullet at all. This time the reload is a sure thing:
                || weapon->GetBullets() < weapon->GetShotCost()
                && weapon->GetBullets() < weapon->GetShotCostAlt()))
        {
            weapon->Reload();
            dirty = true;
        }
    }

    weapon->SetReloadTime( weapon->GetReloadTime() - DeltaTime );

    if (mProgramState.mMode != core::ProgramState::Client)
    {
        if (weapon->GetBullets() <= 0.0)
        {
            if (weapon->GetReloadTime() <= 0 && weapon->GetStaticReload() == 0.0)
            {
                //todo: need to sync reloading with the server (-1 could occur, if a shot comes too fast, then it is reset by the end of the reload missing one bullet)
                weapon->SetBullets( weapon->GetNextReloadBulletCount() );
                weapon->SetNextReloadBulletCount( 0.0 );
                weapon->SetReloadTime( 0.0 );
                dirty = true;
            }
        }

        if (weapon->GetReloadTime() <= 0 && weapon->GetStaticReload() > 0.0)
        {
            weapon->StaticReload();
            dirty = true;
        }
    }
    BindIds_t::iterator itemssIt = mSubSystems.get<SubSystemHolder::AllByBindId>().find( weapon->GetId() );
    if ( itemssIt != mSubSystems.get<SubSystemHolder::AllByBindId>().end() )
    {
        itemssIt->mSystem->Update( actor, DeltaTime );
    }

    if (mProgramState.mMode != core::ProgramState::Client)
    {
        if (weapon->GetCooldown() <= 0.0) //not synced to client
        {
            Opt<IPositionComponent> actorPositionC = actor.Get<IPositionComponent>();
            if (actorPositionC.IsValid())
            {
                if (weapon->IsShooting() && weapon->GetBullets() >= weapon->GetShotCost())
                {
                    if (weapon->GetMuzzleId() != -1)
                    {
                        std::auto_ptr<Actor> muzzle(mActorFactory(weapon->GetMuzzleId()));
                        SetProjectilePosition(*muzzle, actor);
                        BOOST_ASSERT(muzzle->Get<IPositionComponent>().IsValid());
                        muzzle->Get<IPositionComponent>()->SetOrientation(actorPositionC->GetOrientation());
                        mScene.AddActor(muzzle.release());
                    }
                    EventServer<core::ShotEvent>::Get().SendEvent(core::ShotEvent(actor.GetGUID(), glm::vec2(actorPositionC->GetX(), actorPositionC->GetY()), false));
                }
                else if (weapon->IsShootingAlt() && weapon->GetBullets() >= weapon->GetShotCostAlt())
                {
                    if (weapon->GetMuzzleAltId() != -1)
                    {
                        std::auto_ptr<Actor> muzzle(mActorFactory(weapon->GetMuzzleAltId()));
                        SetProjectilePosition(*muzzle, actor);
                        BOOST_ASSERT(muzzle->Get<IPositionComponent>().IsValid());
                        muzzle->Get<IPositionComponent>()->SetOrientation(actorPositionC->GetOrientation());
                        mScene.AddActor(muzzle.release());
                    }
                    EventServer<core::ShotEvent>::Get().SendEvent(core::ShotEvent(actor.GetGUID(), glm::vec2(actorPositionC->GetX(), actorPositionC->GetY()), true));
                }
            }
        }
    }
    if (dirty)
    {
        EventServer<ItemPropertiesChangedEvent>::Get().SendEvent( ItemPropertiesChangedEvent( *weapon ) );
    }
}

void WeaponItemSubSystem::OnShot( core::ShotEvent const& Evt )
{
    Opt<Actor> actor( mScene.GetActor( Evt.mActorGUID ) );
    if ( !actor.IsValid() )
    {
        return;
    }
    Opt<IInventoryComponent> inventoryC = actor->Get<IInventoryComponent>();
    if ( !inventoryC.IsValid() )
    {
        return;
    }
    Opt<Weapon> weapon = inventoryC->GetSelectedItem( ItemType::Weapon );
    if ( !weapon.IsValid() )
    {
        return;
    }
    weapon->GetScatter().Shot( Evt.mIsAlt );
    if ( Evt.mIsAlt )
    {
        weapon->SetCooldown( weapon->GetShootAltCooldown() );
        weapon->SetBullets( weapon->GetBullets() - weapon->GetShotCostAlt() );
    }
    else
    {
        weapon->SetCooldown( weapon->GetShootCooldown() );
        weapon->SetBullets( weapon->GetBullets() - weapon->GetShotCost() );
    }
}


void WeaponItemSubSystem::AddProjectiles( Actor& actor, Projectiles_t& projectiles, Scatter& scatter, bool alt/*=false*/ )
{
    double actorOrientation = actor.Get<IPositionComponent>()->GetOrientation();
    int scat = int( scatter.GetCalculated() * 1000 );
    if( scat )
    {
        double realScatter = ( RandomGenerator::global()() % ( scat + 1 ) - scat / 2. );
        L2( "realScatter:%f\n", realScatter );
        actorOrientation += ( RandomGenerator::global()() % ( scat + 1 ) - scat / 2. ) * 0.001 * boost::math::double_constants::pi;
    }

    L2( "calculated and updated scatter:%f\n", scatter.GetCalculated() );
    Opt<IPositionComponent> actorPositionC = actor.Get<IPositionComponent>();
    for( Projectiles_t::iterator i = projectiles.begin(), e = projectiles.end(); i != e; ++i )
    {
        Actor& Proj = **i;
        SetProjectilePosition( Proj, actor );
        Opt<ShotCollisionComponent> shotCC = Proj.Get<ICollisionComponent>();
        if ( shotCC.IsValid() )
        {
            shotCC->SetParentGUID( actor.GetGUID() );
        }
        Opt<IOwnerComponent> ownerC = Proj.Get<IOwnerComponent>();
        if ( ownerC.IsValid() && ownerC->GetOwnerGUID() == -1 ) //if proj owner is set, then not the given actor is the owner
        {
            ownerC->SetOwnerGUID( actor.GetGUID() );
        }

        Opt<IPositionComponent> projPositionC = Proj.Get<IPositionComponent>();
        projPositionC->SetOrientation( projPositionC->GetOrientation() + actorOrientation );
        Opt<IMoveComponent> moveC( Proj.Get<IMoveComponent>() );
        if ( moveC.IsValid() )
        {
            moveC->SetHeading( projPositionC->GetOrientation() );
        }
        mScene.AddActor( &Proj );
    }
}

Opt<WeaponItemSubSystem> WeaponItemSubSystem::Get()
{
    return Opt<WeaponItemSubSystem>(
               Engine::Get().GetSystem<InventorySystem>()->GetSubSystem( ItemType::Weapon ) );
}
void WeaponItemSubSystem::SetProjectilePosition(Actor& projectile, Actor& actor)
{
    Opt<IPositionComponent> projPositionC = projectile.Get<IPositionComponent>();
    Opt<IPositionComponent> actorPositionC = actor.Get<IPositionComponent>();
    glm::vec2 rvec(0.0, 0.0);
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    if (inventoryC.IsValid())
    {
        Opt<Weapon> weapon = inventoryC->GetSelectedItem( ItemType::Weapon );
        if (weapon.IsValid())
        {
            rvec.x += weapon->GetPositionX();
            rvec.y -= weapon->GetPositionY();
        }
    }
    rvec=glm::rotate(rvec, float(actorPositionC->GetOrientation()));
    projPositionC->SetX( projPositionC->GetX() + actorPositionC->GetX() + rvec.x );
    projPositionC->SetY( projPositionC->GetY() + actorPositionC->GetY() + rvec.y );
}
} // namespace engine

