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
    std::auto_ptr<Item> a = mItemFactory( Id );
    if (mActor)
    {
        a->SetActor(mActor);
    }
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

void InventoryComponent::DropItemType( ItemType::Type Type )
{
    for( ItemList_t::iterator i = mItems.begin(), e = mItems.end(), n; ( i != e ? ( n = i, ++n, true ) : false ); i = n )
    {
        if( (*i)->GetType() == Type )
        {
            delete (*i).Get();
            mItems.erase( i );
        } 
    }
    if (Type==ItemType::Weapon)//TODO: handle multiple items, and handle this situation
    {
        SetSelectedWeapon( -1 );
    }
    else if (Type==ItemType::Normal)
    {
        SetSelectedNormalItem( -1 );
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

void InventoryComponent::SetActor(Actor* Obj)
{
    IInventoryComponent::SetActor(Obj);
    for(ItemList_t::iterator i=mItems.begin(), e=mItems.end();i!=e;++i)
    {
        (*i)->SetActor( mActor );
    }

}

Opt<NormalItem> InventoryComponent::GetSelectedNormalItem()
{
    return mSelectedNormalItem;
}

void InventoryComponent::SetSelectedNormalItem(int32_t Id)
{
    mSelectedNormalItem=Opt<NormalItem>(dynamic_cast<NormalItem*>(GetItem(Id).Get()));
}

void InventoryComponentLoader::BindValues()
{
    std::string istr;
    //TODO: handle more than one items (additem in an array nothing much)
    if( Json::GetStr( (*mSetters)["add_item"], istr))
    {
        Bind<int32_t>(&InventoryComponent::AddItem,AutoId(istr));
    }
}

InventoryComponentLoader::InventoryComponentLoader()
{

}
