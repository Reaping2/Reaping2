#include "platform/i_platform.h"
#include "core/shoot_action.h"
#include "core/i_inventory_component.h"
#include "core/weapon.h"

ShootAction::ShootAction( int32_t Id )
    : Action( Id )
{
    mAreBlockedActionsExcluded = false;
    mCancelledActionIds.push_back( AutoId( "shoot_alt" ) );
    mSecsToEnd = 1;
    mIsLoop = true;
    mIsSelfDestruct = false;
    mIsRefresh = false;
}

ShootAltAction::ShootAltAction( int32_t Id )
    : Action( Id )
{
    mAreBlockedActionsExcluded = false;
    mCancelledActionIds.push_back( AutoId( "shoot" ) );
    mSecsToEnd = 1;
    mIsLoop = true;
    mIsSelfDestruct = false;
    mIsRefresh = false;
}

void ShootAction::Update( double Seconds )
{
    if( !mActor )
    {
        return;
    }
    //TODO: definitely need a nicer way to get weapons, or any item.
    Opt<IInventoryComponent> inventoryC = mActor->Get<IInventoryComponent>();
    if(inventoryC.IsValid())
    {
        IInventoryComponent::ItemList_t const& items = inventoryC->GetItems();
        for( IInventoryComponent::ItemList_t::const_iterator i = items.begin(), e = items.end(); i != e; ++i )
        {
            if (i->GetType()==Item::Weapon)
            {
                Weapon& weapon = static_cast<Weapon&>(const_cast<Item&>(*i));
                weapon.Shoot();
            }
        }
    }
}
void ShootAltAction::Update( double Seconds )
{
    if( !mActor )
    {
        return;
    }
    Opt<IInventoryComponent> inventoryC = mActor->Get<IInventoryComponent>();
    if(inventoryC.IsValid())
    {
        IInventoryComponent::ItemList_t const& items = inventoryC->GetItems();
        for( IInventoryComponent::ItemList_t::const_iterator i = items.begin(), e = items.end(); i != e; ++i )
        {
            if (i->GetType()==Item::Weapon)
            {
                Weapon& weapon = static_cast<Weapon&>(const_cast<Item&>(*i));
                weapon.ShootAlt();
            }
        }
    }
}