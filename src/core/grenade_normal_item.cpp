#include "grenade_normal_item.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

GrenadeNormalItem::GrenadeNormalItem( int32_t Id )
    : NormalItem( Id )
{

}

GrenadeNormalItem::GrenadeNormalItem()
    : NormalItem( -1 )
{

}

REAPING2_CLASS_EXPORT_IMPLEMENT( GrenadeNormalItem, GrenadeNormalItem );
