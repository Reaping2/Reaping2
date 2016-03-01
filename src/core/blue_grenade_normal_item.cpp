#include "core/blue_grenade_normal_item.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

BlueGrenadeNormalItem::BlueGrenadeNormalItem( int32_t id )
    : NormalItem(id)
{
}

BlueGrenadeNormalItem::BlueGrenadeNormalItem()
    : NormalItem(-1)
{
}



REAPING2_CLASS_EXPORT_IMPLEMENT(BlueGrenadeNormalItem, BlueGrenadeNormalItem);
