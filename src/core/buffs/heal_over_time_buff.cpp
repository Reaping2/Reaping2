#include "platform/i_platform.h"
#include "core/buffs/heal_over_time_buff.h"

HealOverTimeBuff::HealOverTimeBuff()
    : Buff()
    , mHealPerTick( 5 )
{
    mSecsToEnd = 4.0;
    mFrequencyTimer.SetFrequency( 0.333 );
}

void HealOverTimeBuff::SetHealPerTick( int32_t healPerTick )
{
    mHealPerTick = healPerTick;
}

int32_t HealOverTimeBuff::GetHealPerTick()const
{
    return mHealPerTick;
}

platform::FrequencyTimer& HealOverTimeBuff::GetFrequencyTimer()
{
    return mFrequencyTimer;
}
