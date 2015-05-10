#ifndef INCLUDED_CORE_BUFFS_HEAL_OVER_TIME_BUFF_H
#define INCLUDED_CORE_BUFFS_HEAL_OVER_TIME_BUFF_H

#include "buff.h"
#include "platform\frequency_timer.h"

class HealOverTimeBuff : public Buff
{
public:
    DEFINE_BUFF_BASE(HealOverTimeBuff)
    HealOverTimeBuff();
    void SetHealPerTick(int32_t healPerTick);
    int32_t GetHealPerTick()const;
    platform::FrequencyTimer& GetFrequencyTimer();
protected:
    int32_t mHealPerTick;
    platform::FrequencyTimer mFrequencyTimer;
};

#endif//INCLUDED_CORE_BUFFS_HEAL_OVER_TIME_BUFF_H
