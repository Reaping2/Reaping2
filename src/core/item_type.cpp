#include "core/item_type.h"
#include "platform/auto_id.h"

using platform::AutoId;

ItemType::ItemType()
{
    mIdToItemTypeMap.insert(IdToItemTypeMap_t::value_type(AutoId("normal"),ItemType::Normal));
    mIdToItemTypeMap.insert(IdToItemTypeMap_t::value_type(AutoId("weapon"),ItemType::Weapon));
    mIdToItemTypeMap.insert(IdToItemTypeMap_t::value_type(AutoId("buff"),ItemType::Buff));
}

ItemType::Type ItemType::operator()( int32_t Id ) const
{
    IdToItemTypeMap_t::left_const_iterator i=mIdToItemTypeMap.left.find(Id);
    BOOST_ASSERT(i!=mIdToItemTypeMap.left.end());
    return (i!=mIdToItemTypeMap.left.end())?i->second:ItemType::Normal;
}

int32_t ItemType::operator()(Type type) const
{
    IdToItemTypeMap_t::right_const_iterator i=mIdToItemTypeMap.right.find(type);
    BOOST_ASSERT(i!=mIdToItemTypeMap.right.end());
    return (i!=mIdToItemTypeMap.right.end())?i->second:AutoId("normal");
}

