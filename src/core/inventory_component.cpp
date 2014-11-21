#include "inventory_component.h"

InventoryComponent::InventoryComponent()
    : mActor( NULL )
    , mItemFactory( ItemFactory::Get() )
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

void InventoryComponent::SetActor( Actor* Obj )
{
    mActor=Obj;
}