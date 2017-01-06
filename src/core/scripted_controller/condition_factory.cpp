#include "platform/i_platform.h"
#include "condition_factory.h"
#include "is_hit_condition.h"
#include "distance_to_aggressor_condition.h"

using platform::AutoId;
namespace scriptedcontroller {

ConditionFactory::ConditionFactory()
{
    Bind( AutoId("default_condition"), &CreateICondition<DefaultCondition> );
    SetDefault( AutoId("default_condition") );

    Bind( AutoId( "is_hit" ), &CreateICondition<IsHitCondition> );
    Bind( AutoId( "distance_to_aggressor" ), &CreateICondition<DistanceToAggressorCondition> );

}

} // namespace scriptedcontroller

