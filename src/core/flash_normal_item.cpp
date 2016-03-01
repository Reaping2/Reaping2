#include "core/flash_normal_item.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

FlashNormalItem::FlashNormalItem( int32_t id )
    : NormalItem(id)
{
}

FlashNormalItem::FlashNormalItem()
    : NormalItem(-1)
{
}



REAPING2_CLASS_EXPORT_IMPLEMENT(FlashNormalItem, FlashNormalItem);
