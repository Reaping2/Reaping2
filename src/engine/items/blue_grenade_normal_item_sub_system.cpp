#include "engine/items/blue_grenade_normal_item_sub_system.h"
#include "normal_item_sub_system.h"
#include "grenade_normal_item_sub_system.h"

namespace engine {

BlueGrenadeNormalItemSubSystem::BlueGrenadeNormalItemSubSystem()
    : mScene( Scene::Get() )
    , mNormalItemSubSystem( NormalItemSubSystem::Get() )
    , mActorFactory( ActorFactory::Get() )
    , mProjectileId( AutoId( "blue_grenade_projectile" ) )
{
}


void BlueGrenadeNormalItemSubSystem::Init()
{
}


void BlueGrenadeNormalItemSubSystem::Update( Actor& actor, double DeltaTime )
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<NormalItem> normalItem = inventoryC->GetSelectedItem( ItemType::Normal );
    if ( normalItem->IsUse() )
    {
        std::auto_ptr<Actor> Proj = mActorFactory( mProjectileId );
        GrenadeNormalItemSubSystem::SetGrenadeProperties( *Proj.get(), actor );

        mScene.AddActor( Proj.release() );
        normalItem->SetConsumed( true );
    }
}

} // namespace engine

