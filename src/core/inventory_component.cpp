#include "inventory_component.h"

InventoryComponent::InventoryComponent()
    : mItemFactory( ItemFactory::Get() )
{

}

void InventoryComponent::Update(double Seconds)
{
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
    mItems.push_back( Opt<Item>(a.release()) );
}

Opt<Item> InventoryComponent::GetItem(int32_t Id)
{
    for( ItemList_t::iterator i = mItems.begin(), e = mItems.end(); i != e; ++i )
    {
        if ((*i)->GetId()==Id)
        {
            return *i;
        }
    }
    return Opt<Item>();
}

void InventoryComponent::DropItemType( Item::ItemType Type )
{
    for( ItemList_t::iterator i = mItems.begin(), e = mItems.end(), n; ( i != e ? ( n = i, ++n, true ) : false ); i = n )
    {
        if( (*i)->GetType() == Type )
        {
            delete (*i).Get();
            mItems.erase( i );
        } 
    }
}

Opt<Weapon> InventoryComponent::GetSelectedWeapon()
{
    return mSelectedWeapon;
}

void InventoryComponent::SetSelectedWeapon(int32_t Id)
{
    mSelectedWeapon=Opt<Weapon>(dynamic_cast<Weapon*>(GetItem(Id).Get()));
}

InventoryComponent::~InventoryComponent()
{
    for( ItemList_t::iterator i = mItems.begin(), e = mItems.end(), n; ( i != e ? ( n = i, ++n, true ) : false ); i = n )
    {
        delete (*i).Get();
    }
    mItems.clear();
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
