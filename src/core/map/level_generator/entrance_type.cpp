#include "core/map/level_generator/entrance_type.h"
#include "platform/auto_id.h"

using platform::AutoId;

namespace map {

EntranceType::EntranceType()
{
    mIdToEntranceTypeMap.insert(IdToEntranceTypeMap_t::value_type(AutoId("top"),EntranceType::Top));
    mIdToEntranceTypeMap.insert(IdToEntranceTypeMap_t::value_type(AutoId("right"),EntranceType::Right));
    mIdToEntranceTypeMap.insert(IdToEntranceTypeMap_t::value_type(AutoId("bottom"),EntranceType::Bottom));
    mIdToEntranceTypeMap.insert(IdToEntranceTypeMap_t::value_type(AutoId("left"),EntranceType::Left));
}

EntranceType::Type EntranceType::operator()( int32_t Id ) const
{
    IdToEntranceTypeMap_t::left_const_iterator i=mIdToEntranceTypeMap.left.find(Id);
    BOOST_ASSERT(i!=mIdToEntranceTypeMap.left.end());
    return (i!=mIdToEntranceTypeMap.left.end())?i->second:EntranceType::Top;
}

int32_t EntranceType::operator()( Type type ) const
{
    IdToEntranceTypeMap_t::right_const_iterator i=mIdToEntranceTypeMap.right.find(type);
    BOOST_ASSERT(i!=mIdToEntranceTypeMap.right.end());
    return (i!=mIdToEntranceTypeMap.right.end())?i->second:EntranceType::Top;
}

} // namespace map

