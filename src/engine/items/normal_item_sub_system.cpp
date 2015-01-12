#include "platform/i_platform.h"
#include "engine/items/normal_item_sub_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "core/i_inventory_component.h"

namespace engine {

NormalItemSubSystem::NormalItemSubSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
{

}

void NormalItemSubSystem::Init()
{
    SubSystemHolder::Init();
}

void NormalItemSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    IInventoryComponent::ItemList_t& items = inventoryC->GetItems();
    for( IInventoryComponent::ItemList_t::iterator i = items.begin(), e = items.end(); i != e; ++i )
    {
        if (i->GetType()!=Item::Normal)
        {
            continue;
        }

        BindIds_t::iterator itemssIt=mSubSystems.get<SubSystemHolder::AllByBindId>().find(i->GetId());
        if (itemssIt!=mSubSystems.get<SubSystemHolder::AllByBindId>().end())
        {
            itemssIt->mSystem->Update(actor,DeltaTime);
        }
    }
}

} // namespace engine

