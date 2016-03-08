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
    L1( "normal item sub system init" );
}

void NormalItemSubSystem::Update( Actor& actor, double DeltaTime )
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    IInventoryComponent::ItemList_t& items = inventoryC->GetItems();
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
            inventoryC->DropItemType( ItemType::Normal );
            Opt<Weapon> weapon = inventoryC->GetSelectedWeapon();
            EventServer<ItemChangedEvent>::Get().SendEvent( ItemChangedEvent( actor.GetGUID(), 0, weapon.IsValid() ? weapon->GetId() : 0 ) );
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

