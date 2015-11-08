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

namespace engine {

WeaponItemSubSystem::WeaponItemSubSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
{

}

void WeaponItemSubSystem::Init()
{
    SubSystemHolder::Init();
}

void WeaponItemSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<Weapon> weapon = inventoryC->GetSelectedWeapon();
    if (!weapon.IsValid())
    {
        return;
    }
    double cd = weapon->GetCooldown();
    cd -= DeltaTime;
    if( cd < 0 )
    {
        cd = 0;
    }
    weapon->SetCooldown(cd);

    BindIds_t::iterator itemssIt=mSubSystems.get<SubSystemHolder::AllByBindId>().find(weapon->GetId());
    if (itemssIt!=mSubSystems.get<SubSystemHolder::AllByBindId>().end())
    {
        itemssIt->mSystem->Update(actor,DeltaTime);
    }

    if (weapon->GetCooldown()==0)
    {
        if (weapon->IsShoot())
        {
            weapon->SetCooldown(weapon->GetShootCooldown());
        }
        else if (weapon->IsShootAlt())
        {
            weapon->SetCooldown(weapon->GetShootAltCooldown());
        }
    }
}

void WeaponItemSubSystem::AddProjectiles(Actor& actor, Projectiles_t& projectiles, uint32_t scatter)
{
    double actorOrientation = actor.Get<IPositionComponent>()->GetOrientation();
    Opt<IAccuracyComponent> accuracyC=actor.Get<IAccuracyComponent>();
    scatter*=100;
    if(accuracyC.IsValid())
    {
        scatter=scatter*(100.0/(100.0+accuracyC->GetAccuracy().Get()));
    }
    if( scatter )
    {
        actorOrientation += ( rand() % (scatter+1) - scatter / 2. ) * 0.0001 * boost::math::double_constants::pi;
    }

    Opt<IPositionComponent> actorPositionC = actor.Get<IPositionComponent>();
    Opt<ICollisionComponent> collisionC = actor.Get<ICollisionComponent>();
    for( Projectiles_t::iterator i = projectiles.begin(), e = projectiles.end(); i != e; ++i )
    {
        Actor& Proj = **i;
        SetProjectilePosition(Proj,actor);
        Opt<ShotCollisionComponent> shotCC=Proj.Get<ShotCollisionComponent>();
        if (shotCC.IsValid())
        {
            shotCC->SetParentGUID( actor.GetGUID() );
        }
        Opt<IPositionComponent> projPositionC = Proj.Get<IPositionComponent>();
        projPositionC->SetOrientation( projPositionC->GetOrientation() + actorOrientation );
        Proj.Get<IMoveComponent>()->SetHeading( projPositionC->GetOrientation() );
        mScene.AddActor( &Proj );
    }
}

Opt<WeaponItemSubSystem> WeaponItemSubSystem::Get()
{
    return Opt<WeaponItemSubSystem>(
        Engine::Get().GetSystem<InventorySystem>()->GetSubSystem(ItemType::Weapon));
}
void WeaponItemSubSystem::SetProjectilePosition(Actor& projectile, Actor& actor)
{
    Opt<IPositionComponent> projPositionC = projectile.Get<IPositionComponent>();
    Opt<IPositionComponent> actorPositionC = actor.Get<IPositionComponent>();
    Opt<IMoveComponent> actorMoveC = actor.Get<IMoveComponent>();
    const double h = actorPositionC->GetOrientation();
    const double c = cos( h );
    const double s = sin( h );
    Opt<ICollisionComponent> collisionC = actor.Get<ICollisionComponent>();
    double radius=collisionC.IsValid()?collisionC->GetRadius():0.0;
    projPositionC->SetX( actorPositionC->GetX()+c*radius);
    projPositionC->SetY( actorPositionC->GetY()+s*radius);
//     double actorSpeedX=actorMoveC.IsValid()?actorMoveC->GetSpeedX():0.0;
//     double actorSpeedY=actorMoveC.IsValid()?actorMoveC->GetSpeedY():0.0;
//     Opt<ICollisionComponent> collisionC = actor.Get<ICollisionComponent>();
//     double radius=collisionC.IsValid()?collisionC->GetRadius():0.0;
//     double actorSpeedXNormalized=actorMoveC->GetSpeed()==0.0?0.0:actorSpeedX/abs(actorMoveC->GetSpeed());
//     double actorSpeedYNormalized=actorMoveC->GetSpeed()==0.0?0.0:actorSpeedY/abs(actorMoveC->GetSpeed());
//     projPositionC->SetX( actorPositionC->GetX()+actorSpeedXNormalized*radius);
//     projPositionC->SetY( actorPositionC->GetY()+actorSpeedYNormalized*radius);
}
} // namespace engine

