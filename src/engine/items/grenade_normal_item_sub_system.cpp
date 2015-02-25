#include "engine/items/grenade_normal_item_sub_system.h"
#include "weapon_item_sub_system.h"

namespace engine {

GrenadeNormalItemSubSystem::GrenadeNormalItemSubSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
    , mWeaponItemSubSystem(WeaponItemSubSystem::Get())
    , mActorFactory(ActorFactory::Get())
    , mProjectileId( AutoId( "grenade_projectile" ) )
{
}

void GrenadeNormalItemSubSystem::Init()
{
    SubSystemHolder::Init();
}

void GrenadeNormalItemSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<NormalItem> normalItem = inventoryC->GetSelectedNormalItem();
    if (normalItem->IsUse())
    {
        WeaponItemSubSystem::Projectiles_t projectiles;

        std::auto_ptr<Actor> ps = mActorFactory(mProjectileId);
        projectiles.push_back( Opt<Actor>(ps.release()) );

        mWeaponItemSubSystem->AddProjectiles(actor,projectiles,0);
        normalItem->SetConsumed(true);
    }
}

} // namespace engine

