#include "platform/i_platform.h"
#include "engine/items/normal_item_sub_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "core/i_inventory_component.h"
#include "inventory_system.h"
#include "../item_changed_event.h"

namespace engine {

NormalItemSubSystem::NormalItemSubSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
{

}

void NormalItemSubSystem::Init()
{
    SubSystemHolder::Init();
    L2( "normal item sub system init" );
}

void NormalItemSubSystem::Update( Actor& actor, double DeltaTime )
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    Opt<NormalItem> normalItem = inventoryC->GetSelectedNormalItem();
    if ( !normalItem.IsValid() )
    {
        return;
    }
    BindIds_t::iterator itemssIt = mSubSystems.get<SubSystemHolder::AllByBindId>().find( normalItem->GetId() );
    if ( itemssIt != mSubSystems.get<SubSystemHolder::AllByBindId>().end() )
    {
        itemssIt->mSystem->Update( actor, DeltaTime );
        if ( normalItem->IsConsumed() )
        {
            int32_t id = normalItem->GetId();
            inventoryC->SwitchToNextItem( ItemType::Normal );
            inventoryC->DropItem( id );
            Opt<NormalItem> currItem = inventoryC->GetSelectedNormalItem();
            if (currItem.IsValid())
            {
                EventServer<ItemChangedEvent>::Get().SendEvent( ItemChangedEvent( actor.GetGUID(), ItemType::Normal, currItem->GetId(), id, true ) );
            }
        }
    }

    // NO MORE ACTIONS HERE. Item is dropped!


    /*
    for( IInventoryComponent::ItemList_t::iterator i = items.begin(), e = items.end(); i != e; ++i )
    {
        if ((*i)->GetType()!=Item::Normal)
        {
            continue;
        }

        BindIds_t::iterator itemssIt=mSubSystems.get<SubSystemHolder::AllByBindId>().find((*i)->GetId());
        if (itemssIt!=mSubSystems.get<SubSystemHolder::AllByBindId>().end())
        {
            itemssIt->mSystem->Update(actor,DeltaTime);
        }
    }*/
}

Opt<NormalItemSubSystem> NormalItemSubSystem::Get()
{
    return Opt<NormalItemSubSystem>(
               Engine::Get().GetSystem<InventorySystem>()->GetSubSystem( ItemType::Normal ) );
}

} // namespace engine

