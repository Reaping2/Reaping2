#include "platform/i_platform.h"
#include "buff_factory.h"
#include "heal_over_time_buff.h"
#include "move_speed_buff.h"
#include "max_health_buff.h"
#include "accuracy_buff.h"

using platform::AutoId;
namespace core {

BuffFactory::BuffFactory()
{
    Bind( DefaultBuff::GetType_static(), &CreateBuff<DefaultBuff> );
    SetDefault( DefaultBuff::GetType_static() );

    Bind( HealOverTimeBuff::GetType_static(), &CreateBuff<HealOverTimeBuff> );
    Bind( MoveSpeedBuff::GetType_static(), &CreateBuff<MoveSpeedBuff> );
    Bind( MaxHealthBuff::GetType_static(), &CreateBuff<MaxHealthBuff> );
    Bind( AccuracyBuff::GetType_static(), &CreateBuff<AccuracyBuff> );
}

} // namespace core

