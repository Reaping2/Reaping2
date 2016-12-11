#include "platform/i_platform.h"
#include "condition_factory.h"

using platform::AutoId;
namespace scriptedcontroller {

ConditionFactory::ConditionFactory()
{
    Bind( AutoId("default_condition"), &CreateICondition<DefaultCondition> );
    SetDefault( AutoId("default_condition") );

}

} // namespace scriptedcontroller

