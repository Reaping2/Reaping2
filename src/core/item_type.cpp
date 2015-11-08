#include "core/item_type.h"
#include "platform/auto_id.h"

using platform::AutoId;

ItemType::ItemType()
{
    mIdToItemTypeMap[AutoId("normal")]=ItemType::Normal;
    mIdToItemTypeMap[AutoId("weapon")]=ItemType::Weapon;
    mIdToItemTypeMap[AutoId("buff")]=ItemType::Buff;
}

ItemType::Type ItemType::operator()( int32_t Id ) const
{
    IdToItemTypeMap_t::const_iterator i=mIdToItemTypeMap.find(Id);
    BOOST_ASSERT(i!=mIdToItemTypeMap.end());
    return (i!=mIdToItemTypeMap.end())?i->second:ItemType::Normal;
}

