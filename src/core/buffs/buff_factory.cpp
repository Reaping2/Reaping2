#include "platform/i_platform.h"
#include "buff_factory.h"
#include "heal_over_time_buff.h"
#include "move_speed_buff.h"
#include "max_health_buff.h"

using platform::AutoId;
namespace core {

BuffFactory::BuffFactory()
{
    Bind(AutoId("default_buff"), &CreateBuff<DefaultBuff> );
    SetDefault(AutoId("default_buff"));

    Bind(AutoId("heal_over_time_buff"), &CreateBuff<HealOverTimeBuff> );
    Bind(AutoId("move_speed_buff"), &CreateBuff<MoveSpeedBuff> );
    Bind(AutoId("max_health_buff"), &CreateBuff<MaxHealthBuff> );
}

} // namespace core

