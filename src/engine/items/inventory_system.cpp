#include "platform/i_platform.h"
#include "engine/items/inventory_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "core/i_inventory_component.h"
#include "core/i_health_component.h"
#include "../system_suppressor.h"
namespace engine {

InventorySystem::InventorySystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
{

}

void InventorySystem::Init()
{
    SystemSuppressor::Get().Add( SystemSuppressor::SceneLoad, GetType_static() );

    SubSystemHolder::Init();
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IInventoryComponent>().IsValid()
            && actor.Get<IHealthComponent>().IsValid(); } );
}

void InventorySystem::Update( double DeltaTime )
{
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
        Opt<IInventoryComponent> inventoryC = actor->Get<IInventoryComponent>();
        if ( !inventoryC.IsValid() )
        {
            continue;
        }
        Opt<IHealthComponent> healthC = actor->Get<IHealthComponent>();
        if( healthC.IsValid() && !healthC->IsAlive() )
        {
            //if it has no healthComponent, it should not
            continue;
        }

        for( auto ss : mSubSystems )
        {
            ss.mSystem->Update( *actor, DeltaTime );
        }
    }
    mScene.InsertNewActors();
}


} // namespace engine

