#include "inventory_component.h"

InventoryComponent::InventoryComponent()
    : mItemFactory( ItemFactory::Get() )
{

}

void InventoryComponent::Update(double Seconds)
{
    for( ItemList_t::iterator i = mItems.begin(), e = mItems.end(); i != e; ++i )
    {
        i->Update( Seconds );
    }
}

InventoryComponent::ItemList_t const& InventoryComponent::GetItems()const
{
    return mItems;
}

InventoryComponent::ItemList_t& InventoryComponent::GetItems()
{
    return const_cast<ItemList_t&>((static_cast<const InventoryComponent*>(this))->GetItems());
}

void InventoryComponent::AddItem( int32_t Id )
{
    BOOST_ASSERT(mActor);
    std::auto_ptr<Item> a = mItemFactory( Id );
    a->SetActor( mActor );
    mItems.push_back( a );
}

void InventoryComponent::DropItemType( Item::ItemType Type )
{
    for( ItemList_t::iterator i = mItems.begin(), e = mItems.end(), n; ( i != e ? ( n = i, ++n, true ) : false ); i = n )
    {
        if( i->GetType() == Type )
        {
            mItems.erase( i );
        } 
    }
}

void InventoryComponentLoader::BindValues()
{
    std::string istr;
    if( Json::GetStr( (*mSetters)["additem"], istr))
    {
        Bind<int32_t>(&InventoryComponent::AddItem,AutoId(istr));
    }
}

InventoryComponentLoader::InventoryComponentLoader()
{

}
