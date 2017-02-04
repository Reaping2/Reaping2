#include "platform/i_platform.h"
#include "condition_factory.h"
#include "is_hit_condition.h"
#include "distance_to_aggressor_condition.h"
#include "is_not_hit_condition.h"
#include "distance_to_players_condition.h"
#include "distance_to_target_condition.h"
#include "random_condition.h"

using platform::AutoId;
namespace scriptedcontroller {

ConditionFactory::ConditionFactory()
{
    Bind( AutoId("default_condition"), &CreateICondition<DefaultCondition> );
    SetDefault( AutoId("default_condition") );

    Bind( AutoId( "is_hit" ), &CreateICondition<IsHitCondition> );
    Bind( AutoId( "is_not_hit" ), &CreateICondition<IsNotHitCondition> );
    Bind( AutoId( "distance_to_aggressor" ), &CreateICondition<DistanceToAggressorCondition> );
    Bind( AutoId( "distance_to_players" ), &CreateICondition<DistanceToPlayersCondition> );
    Bind( AutoId( "distance_to_target" ), &CreateICondition<DistanceToTargetCondition> );
    Bind( AutoId( "random" ), &CreateICondition<RandomCondition> );
}

} // namespace scriptedcontroller

