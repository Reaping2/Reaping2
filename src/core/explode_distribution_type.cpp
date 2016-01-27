#include "core/explode_distribution_type.h"
#include "platform/auto_id.h"

using platform::AutoId;

ExplodeDistributionType::ExplodeDistributionType()
{
    mIdToExplodeDistributionTypeMap.insert(IdToExplodeDistributionTypeMap_t::value_type(AutoId("normal"),ExplodeDistributionType::Normal));
    mIdToExplodeDistributionTypeMap.insert(IdToExplodeDistributionTypeMap_t::value_type(AutoId("random"),ExplodeDistributionType::Random));
}

ExplodeDistributionType::Type ExplodeDistributionType::operator()( int32_t Id ) const
{
    IdToExplodeDistributionTypeMap_t::left_const_iterator i=mIdToExplodeDistributionTypeMap.left.find(Id);
    BOOST_ASSERT(i!=mIdToExplodeDistributionTypeMap.left.end());
    return (i!=mIdToExplodeDistributionTypeMap.left.end())?i->second:ExplodeDistributionType::Normal;
}

int32_t ExplodeDistributionType::operator()( Type type ) const
{
    IdToExplodeDistributionTypeMap_t::right_const_iterator i=mIdToExplodeDistributionTypeMap.right.find(type);
    BOOST_ASSERT(i!=mIdToExplodeDistributionTypeMap.right.end());
    return (i!=mIdToExplodeDistributionTypeMap.right.end())?i->second:ExplodeDistributionType::Normal;
}

