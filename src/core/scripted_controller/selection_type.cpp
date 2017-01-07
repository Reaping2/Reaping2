#include "selection_type.h"
#include "platform/auto_id.h"

using platform::AutoId;

namespace scriptedcontroller {

SelectionType::SelectionType()
{
    mIdToSelectionTypeMap.insert(IdToSelectionTypeMap_t::value_type(AutoId("current"),SelectionType::Current));
    mIdToSelectionTypeMap.insert(IdToSelectionTypeMap_t::value_type(AutoId("last_hit_by"),SelectionType::LastHitBy));
    mIdToSelectionTypeMap.insert(IdToSelectionTypeMap_t::value_type(AutoId("closest"),SelectionType::Closest));
}

SelectionType::Type SelectionType::operator()( int32_t Id ) const
{
    IdToSelectionTypeMap_t::left_const_iterator i=mIdToSelectionTypeMap.left.find(Id);
    BOOST_ASSERT(i!=mIdToSelectionTypeMap.left.end());
    return (i!=mIdToSelectionTypeMap.left.end())?i->second:SelectionType::Current;
}

int32_t SelectionType::operator()( Type type ) const
{
    IdToSelectionTypeMap_t::right_const_iterator i=mIdToSelectionTypeMap.right.find(type);
    BOOST_ASSERT(i!=mIdToSelectionTypeMap.right.end());
    return (i!=mIdToSelectionTypeMap.right.end())?i->second:SelectionType::Current;
}

} // namespace scriptedcontroller

