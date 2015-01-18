#include "platform/i_platform.h"
#include "engine/items/inventory_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "core/i_inventory_component.h"
#include "core/i_health_component.h"
namespace engine {

InventorySystem::InventorySystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
{

}


void InventorySystem::Update(double DeltaTime)
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
        Actor& actor = **it;
        Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
        if (!inventoryC.IsValid())
        {
            continue;
        }
        Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
        if(healthC.IsValid()&&!healthC->IsAlive())
        {
            //if it has no healthComponent, it should not
            continue; 
        }

        for( SubSystems_t::iterator ssIt = mSubSystems.begin(), e = mSubSystems.end(); ssIt != e; ++ssIt )
        {
            ssIt->mSystem->Update(actor,DeltaTime);
        }
    }
}

void InventorySystem::Init()
{
    SubSystemHolder::Init();
}

} // namespace engine

