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
    if( scatter )
    {
        actorOrientation += ( rand() % scatter - scatter / 2. ) * 0.01 * boost::math::double_constants::pi;
    }

    Opt<IPositionComponent> actorPositionC = actor.Get<IPositionComponent>();
    for( Projectiles_t::iterator i = projectiles.begin(), e = projectiles.end(); i != e; ++i )
    {
        Actor& Proj = **i;
        Opt<IPositionComponent> projPositionC = Proj.Get<IPositionComponent>();
        projPositionC->SetX( actorPositionC->GetX() );
        projPositionC->SetY( actorPositionC->GetY() );
        Proj.Get<ShotCollisionComponent>()->SetParent( &actor );
        projPositionC->SetOrientation( projPositionC->GetOrientation() + actorOrientation );
        Proj.Get<IMoveComponent>()->SetHeading( projPositionC->GetOrientation() );
        mScene.AddActor( &Proj );
    }
}

Opt<WeaponItemSubSystem> WeaponItemSubSystem::Get()
{
    return Opt<WeaponItemSubSystem>(
        dynamic_cast<WeaponItemSubSystem*>(
        Engine::Get().GetSystem<InventorySystem>()->GetSubSystem(Item::Weapon).Get()));
}

} // namespace engine

