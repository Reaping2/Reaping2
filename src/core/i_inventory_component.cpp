#include "i_inventory_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
#include "item_factory.h"

REAPING2_CLASS_EXPORT_IMPLEMENT( IInventoryComponent, IInventoryComponent );

Opt<Item> IInventoryComponent::ItemGroup::AddItem( std::unique_ptr<Item> item )
{
    auto r = item.get();
    DropItem( item->GetId() );
    mItems.push_back( std::move(item) );
    return r;
}

Opt<Item> IInventoryComponent::ItemGroup::GetItem( int32_t Id )
{
    auto found = std::find_if( mItems.begin(), mItems.end(), [&Id]( auto const& item ) { return item->GetId() == Id; } );
    return found != mItems.end() ? found->get() : nullptr;
}

Opt<Item> IInventoryComponent::ItemGroup::GetSelectedItem()
{
    return mSelectedIndex != InvalidItem ? mItems[mSelectedIndex].get() : nullptr;
}


Opt<Item> IInventoryComponent::ItemGroup::SetSelectedItem( int32_t Id )
{
    if (Id == InvalidItem)
    {
        mSelectedIndex = Id;
        return nullptr;
    }
    auto const found = std::find_if( mItems.begin(), mItems.end(), [&Id]( auto const& item ) { return item->GetId() == Id; } );
    mSelectedIndex = found != mItems.end() ? std::distance( mItems.begin(), found ) : InvalidItem;
    return found != mItems.end() ? found->get() : nullptr;
}

bool IInventoryComponent::ItemGroup::DropItem( int32_t Id )
{
    int32_t selectedItemId = GetSelectedItem().IsValid() ? GetSelectedItem()->GetId() : InvalidItem;
    auto const siz = mItems.size();
    mItems.erase( std::remove_if( mItems.begin(), mItems.end(), [&Id]( auto const& item ) { return item->GetId() == Id; } ), mItems.end() );
    SetSelectedItem( selectedItemId );
    return siz != mItems.size();
}


Opt<Item> IInventoryComponent::ItemGroup::SwitchToNextItem( bool forward /*= true */ )
{
    if (mSelectedIndex == InvalidItem)
    {
        return nullptr;
    }
    forward ? ++mSelectedIndex : --mSelectedIndex;
    mSelectedIndex = ( mSelectedIndex + mItems.size() ) % mItems.size();
    return GetSelectedItem();
}

void IInventoryComponent::ItemGroup::SetActorGUID( int32_t actorGUID )
{
    for (auto&& item : mItems)
    {
        item->SetActorGUID( actorGUID );
    }
}
