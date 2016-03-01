#include "core/cloak_normal_item.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

CloakNormalItem::CloakNormalItem( int32_t id )
    : NormalItem(id)
{
}

CloakNormalItem::CloakNormalItem()
    : NormalItem(-1)
{
}



REAPING2_CLASS_EXPORT_IMPLEMENT(CloakNormalItem, CloakNormalItem);
