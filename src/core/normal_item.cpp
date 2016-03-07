#include "normal_item.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

NormalItem::NormalItem( int32_t Id )
    : Item( Id )
    , mUse( false )
    , mConsumed( false )
{
    mType = ItemType::Normal;
}

NormalItem::NormalItem()
    : Item()
    , mUse( false )
    , mConsumed( false )
{

}

void NormalItem::SetUse( bool use )
{
    mUse = use;
}

bool NormalItem::IsUse() const
{
    return mUse;
}

void NormalItem::SetConsumed( bool consumed )
{
    mConsumed = consumed;
}

bool NormalItem::IsConsumed() const
{
    return mConsumed;
}

REAPING2_CLASS_EXPORT_IMPLEMENT( NormalItem, NormalItem );
