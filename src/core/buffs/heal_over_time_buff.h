#ifndef INCLUDED_CORE_BUFFS_HEAL_OVER_TIME_BUFF_H
#define INCLUDED_CORE_BUFFS_HEAL_OVER_TIME_BUFF_H

#include "buff.h"
#include "platform/frequency_timer.h"

class HealOverTimeBuff : public Buff
{
public:
    DEFINE_BUFF_BASE( HealOverTimeBuff )
    HealOverTimeBuff();
    void SetHealPerTick( int32_t healPerTick );
    int32_t GetHealPerTick()const;
    platform::FrequencyTimer& GetFrequencyTimer();
protected:
    int32_t mHealPerTick;
    platform::FrequencyTimer mFrequencyTimer;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void HealOverTimeBuff::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Buff>( *this );
    ar& mHealPerTick;
    ar& mFrequencyTimer;
}

#endif//INCLUDED_CORE_BUFFS_HEAL_OVER_TIME_BUFF_H
