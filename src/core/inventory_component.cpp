#include "inventory_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
#include "item_dropped_event.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

InventoryComponent::InventoryComponent()
    : mItemFactory( ItemFactory::Get() )
    , mSelectedWeapon( 0 )
    , mSelectedNormalItem( 0 )
    , mPickupItems( true )
{

}

void InventoryComponent::Update( double Seconds )
{
}

InventoryComponent::ItemList_t const& InventoryComponent::GetItems()const
{
    return mItems;
}

InventoryComponent::ItemList_t& InventoryComponent::GetItems()
{
    return const_cast<ItemList_t&>( ( static_cast<const InventoryComponent*>( this ) )->GetItems() );
}

void InventoryComponent::AddItem( int32_t Id )
{
    std::auto_ptr<Item> item = mItemFactory( Id );
    item->SetActorGUID( mActorGUID );
    AddItem( std::unique_ptr<Item>(item.release()) );
}

void InventoryComponent::AddItem( std::unique_ptr<Item> item )
{
    DropItem( item->GetId() );
    mItems.push_back( Opt<Item>( item.release() ) );
}

Opt<Item> InventoryComponent::GetItem( int32_t Id )
{
    for( ItemList_t::iterator i = mItems.begin(); i != mItems.end(); ++i )
    {
        if ( ( *i )->GetId() == Id )
        {
            return *i;
        }
    }
    return Opt<Item>();
}

void InventoryComponent::DropItem( int32_t Id )
{
    if (mSelectedWeapon.IsValid() && mSelectedWeapon->GetId() == Id)
    {
        mSelectedWeapon.Reset();
        SetSelectedWeapon( -1 );
    }
    if (mSelectedNormalItem.IsValid() && mSelectedNormalItem->GetId() == Id)
    {
        mSelectedNormalItem.Reset();
        SetSelectedNormalItem( -1 );
    }
    for ( ItemList_t::iterator i = mItems.begin(); i != mItems.end(); )
    {
        if( ( *i )->GetId() == Id )
        {
            //EventServer<core::ItemDroppedEvent>::Get().SendEvent( core::ItemDroppedEvent(*(*i)) );
            delete ( *i ).Get();
            i = mItems.erase( i );
        }
        else
        {
            ++i;
        }
    }
}

void InventoryComponent::DropItemType( ItemType::Type Type )
{
    if (Type == ItemType::Weapon) //TODO: handle multiple items, and handle this situation
    {
        mSelectedWeapon.Reset();
        SetSelectedWeapon( -1 );
    }
    else if (Type == ItemType::Normal)
    {
        mSelectedNormalItem.Reset();
        SetSelectedNormalItem( -1 );
    }
    for ( ItemList_t::iterator i = mItems.begin(); i != mItems.end(); )
    {
        if ((*i)->GetType() == Type)
        {
            //EventServer<core::ItemDroppedEvent>::Get().SendEvent( core::ItemDroppedEvent( *(*i) ) );
            delete (*i).Get();
            i = mItems.erase( i );
        }
        else
        {
            ++i;
        }
    }
}

Opt<Weapon> InventoryComponent::GetSelectedWeapon()
{
    return mSelectedWeapon;
}

bool InventoryComponent::SetSelectedWeapon( int32_t Id, bool force /*= false*/ )
{
    if ( force || !mSelectedWeapon.IsValid() || mSelectedWeapon->CanSwitch())
    {
        if (mSelectedWeapon.IsValid())
        {
            mSelectedWeapon->Deselected();
        }
        mSelectedWeapon = Opt<Weapon>( dynamic_cast<Weapon*>(GetItem( Id ).Get()) );
        if (mSelectedWeapon.IsValid())
        {
            mSelectedWeapon->Selected();
        }
        return true;
    }
    return false;
}

InventoryComponent::~InventoryComponent()
{
    for( ItemList_t::iterator i = mItems.begin(), e = mItems.end(), n; ( i != e ? ( n = i, ++n, true ) : false ); i = n )
    {
        delete ( *i ).Get();
    }
    mItems.clear();
}

void InventoryComponent::SetActorGUID( int32_t actorGUID )
{
    IInventoryComponent::SetActorGUID( actorGUID );
    for( ItemList_t::iterator i = mItems.begin(), e = mItems.end(); i != e; ++i )
    {
        ( *i )->SetActorGUID( mActorGUID );
    }

}

Opt<NormalItem> InventoryComponent::GetSelectedNormalItem()
{
    return mSelectedNormalItem;
}

void InventoryComponent::SetSelectedNormalItem( int32_t Id )
{
    mSelectedNormalItem = Opt<NormalItem>( dynamic_cast<NormalItem*>( GetItem( Id ).Get() ) );
}

void InventoryComponent::SetPickupItems( bool pickupItems )
{
    mPickupItems = pickupItems;
}

bool InventoryComponent::IsPickupItems() const
{
    return mPickupItems;
}


void InventoryComponentLoader::BindValues()
{
    std::string istr;
    //TODO: handle more than one items (additem in an array nothing much
    auto const& addItem = (*mSetters)["add_item"];
    if (addItem.isArray())
    {
        for (auto&& item : addItem)
        {
            if (item.isString())
            {
                Bind<int32_t>( static_cast<void (InventoryComponent::*)(int32_t)>(&InventoryComponent::AddItem), AutoId( item.asString() ) );
            }
        }
    }
    else
    {
        if (Json::GetStr( (*mSetters)["add_item"], istr ))
        {
            Bind<int32_t>( static_cast<void (InventoryComponent::*)(int32_t)>(&InventoryComponent::AddItem), AutoId( istr ) );
        }
    }
    if (Json::GetStr( (*mSetters)["select_weapon"], istr ))
    {
        Bind<int32_t>( ( boost::bind(&InventoryComponent::SetSelectedWeapon,_1,_2,true )), AutoId( istr ));
    }
    Bind( "pickup_items", func_bool( &InventoryComponent::SetPickupItems ) );
}

InventoryComponentLoader::InventoryComponentLoader()
{

}

REAPING2_CLASS_EXPORT_IMPLEMENT( InventoryComponent, InventoryComponent );
