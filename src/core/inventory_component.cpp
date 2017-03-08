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

InventoryComponent::Items_t const& InventoryComponent::GetItems()const
{
    return mItems;
}

InventoryComponent::Items_t& InventoryComponent::GetItems()
{
    return const_cast<Items_t&>( ( static_cast<const InventoryComponent*>( this ) )->GetItems() );
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
    for( Items_t::iterator i = mItems.begin(); i != mItems.end(); ++i )
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
        SetSelectedWeapon( -1 );
    }
    if (mSelectedNormalItem.IsValid() && mSelectedNormalItem->GetId() == Id)
    {
        SetSelectedNormalItem( -1 );
    }
    for ( Items_t::iterator i = mItems.begin(); i != mItems.end(); )
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
        SetSelectedWeapon( -1 );
    }
    else if (Type == ItemType::Normal)
    {
        SetSelectedNormalItem( -1 );
    }
    for ( Items_t::iterator i = mItems.begin(); i != mItems.end(); )
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
        mSelectedWeapon = GetItem( Id );
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
    for( Items_t::iterator i = mItems.begin(), e = mItems.end(), n; ( i != e ? ( n = i, ++n, true ) : false ); i = n )
    {
        delete ( *i ).Get();
    }
    mItems.clear();
}

void InventoryComponent::SetActorGUID( int32_t actorGUID )
{
    IInventoryComponent::SetActorGUID( actorGUID );
    for( Items_t::iterator i = mItems.begin(), e = mItems.end(); i != e; ++i )
    {
        ( *i )->SetActorGUID( mActorGUID );
    }

}

Opt<NormalItem> InventoryComponent::GetSelectedNormalItem()
{
    return mSelectedNormalItem;
}

bool InventoryComponent::SetSelectedNormalItem( int32_t Id, bool force /*= false */ )
{
    if (force || !mSelectedNormalItem.IsValid() || mSelectedNormalItem->CanSwitch())
    {
        if (mSelectedNormalItem.IsValid())
        {
            mSelectedNormalItem->Deselected();
        }
        mSelectedNormalItem = GetItem( Id );
        if (mSelectedNormalItem.IsValid())
        {
            mSelectedNormalItem->Selected();
        }
        return true;
    }
    return false;
}


bool InventoryComponent::SwitchToNextItem( ItemType::Type itemType, bool forward /*= true */ )
{
    Opt<Item> item( (itemType == ItemType::Normal) ? Opt<Item>(mSelectedNormalItem) : Opt<Item>(mSelectedWeapon) );
    if (item.IsValid())
    {
        auto found = std::find_if( mItems.begin(), mItems.end(), [&]( auto i ) { return i->GetId() == item->GetId(); } );
        auto i = found;
        do
        {
            if ((forward ? ++i : --i) == mItems.end())
            {
                i = mItems.begin();
            }
        } while (i != found && (*i)->GetType() != itemType);
        if (i != found)
        {
            return itemType == ItemType::Normal ? SetSelectedNormalItem( (*i)->GetId() ) : SetSelectedWeapon( (*i)->GetId() );
        }
    }
    return false;
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
