#include "inventory_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
#include "item_dropped_event.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
InventoryComponent::InventoryComponent()
    : mItemFactory( ItemFactory::Get() )
    , mPickupItems( true )
    , mDarkMatters( 0 )
    , mCollectDarkMatter( true )
{

}

InventoryComponent::ItemMap_t const& InventoryComponent::GetItems()const
{
    return mItems;
}

InventoryComponent::ItemMap_t& InventoryComponent::GetItems()
{
    return const_cast<ItemMap_t&>( ( static_cast<const InventoryComponent*>( this ) )->GetItems() );
}

Opt<Item> InventoryComponent::AddItem( int32_t Id )
{
    std::auto_ptr<Item> item = mItemFactory( Id );
    item->SetActorGUID( mActorGUID );
    return AddItem( std::move(std::unique_ptr<Item>(item.release())) );
}

Opt<Item> InventoryComponent::AddItem( std::unique_ptr<Item> item )
{
    return mItems[item->GetType()].AddItem(std::move(item));
}

Opt<Item> InventoryComponent::GetItem( int32_t Id )
{
    for (auto&& pairs : mItems)
    {
        auto item( pairs.second.GetItem( Id ) );
        if (item.IsValid())
        {
            return item;
        }
    }
    return Opt<Item>();
}


Opt<Item> InventoryComponent::GetSelectedItem( ItemType::Type type )
{
    return mItems[type].GetSelectedItem();
}


Opt<Item> InventoryComponent::SetSelectedItem( ItemType::Type type, int32_t Id, bool force /*= false */ )
{
    auto selectedItem(mItems[type].GetSelectedItem());
    if (force || !selectedItem.IsValid() || selectedItem->CanSwitch())
    {
        if (selectedItem.IsValid())
        {
            selectedItem->Deselected();
        }
        selectedItem = mItems[type].SetSelectedItem(Id);
        if (selectedItem.IsValid())
        {
            selectedItem->Selected();
        }
    }
    return selectedItem;
}

bool InventoryComponent::DropItem( int32_t Id )
{
    for (auto&& pairs : mItems)
    {
        if (pairs.second.DropItem( Id ))
        {
            return true;
        }
    }
    return false;
}

InventoryComponent::~InventoryComponent()
{
    mItems.clear();
}

void InventoryComponent::SetActorGUID( int32_t actorGUID )
{
    IInventoryComponent::SetActorGUID( actorGUID );
    for (auto&& pairs : mItems)
    {
        pairs.second.SetActorGUID( actorGUID );
    }

}

Opt<Item> InventoryComponent::SwitchToNextItem( ItemType::Type itemType, bool forward /*= true */ )
{
    return mItems[itemType].SwitchToNextItem( forward );
}

void InventoryComponent::SetPickupItems( bool pickupItems )
{
    mPickupItems = pickupItems;
}

bool InventoryComponent::IsPickupItems() const
{
    return mPickupItems;
}

void InventoryComponent::SetDarkMatters( int32_t darkMatters )
{
    mDarkMatters = darkMatters;
}

int32_t InventoryComponent::GetDarkMatters() const
{
    return mDarkMatters;
}

void InventoryComponent::SetCollectDarkMatter( bool collectDarkMatter )
{
    mCollectDarkMatter = collectDarkMatter;
}

bool InventoryComponent::IsCollectDarkMatter() const
{
    return mCollectDarkMatter;
}

void InventoryComponentLoader::BindValues()
{
    std::string istr;
    auto const& addItem = (*mSetters)["add_item"];
    if (addItem.isArray())
    {
        for (auto&& item : addItem)
        {
            if (item.isString())
            {
                Bind<int32_t>( static_cast<Opt<Item> (InventoryComponent::*)(int32_t)>(&InventoryComponent::AddItem), AutoId( item.asString() ) );
            }
        }
    }
    else
    {
        if (Json::GetStr( (*mSetters)["add_item"], istr ))
        {
            Bind<int32_t>( static_cast<Opt<Item> (InventoryComponent::*)(int32_t)>(&InventoryComponent::AddItem), AutoId( istr ) );
        }
    }
    if (Json::GetStr( (*mSetters)["select_weapon"], istr ))
    {
        Bind<int32_t>( (
            std::bind( 
                std::bind(
                    &InventoryComponent::SetSelectedItem,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,true ), 
                std::placeholders::_1,ItemType::Weapon, std::placeholders::_2)), 
            AutoId( istr ));
    }
    Bind( "pickup_items", func_bool( &InventoryComponent::SetPickupItems ) );
    Bind( "collect_dark_matter", func_bool( &InventoryComponent::SetCollectDarkMatter ) );
}

InventoryComponentLoader::InventoryComponentLoader()
{

}

REAPING2_CLASS_EXPORT_IMPLEMENT( InventoryComponent, InventoryComponent );
