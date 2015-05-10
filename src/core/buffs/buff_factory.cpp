#include "platform/i_platform.h"
#include "buff_factory.h"
#include "heal_over_time_buff.h"

using platform::AutoId;
namespace core {

BuffFactory::BuffFactory()
{
    Bind( AutoId("default_buff"), &CreateBuff<DefaultBuff> );
    SetDefault( AutoId("default_buff") );

    Bind( AutoId("heal_over_time_buff"), &CreateBuff<HealOverTimeBuff> );

}

} // namespace core

