#include "core/buffs/max_health_buff.h"

MaxHealthBuff::MaxHealthBuff()
    : Buff()
    , mFlatBonus( 20 )
    , mPercentBonus( 0.0 )
{
    mSecsToEnd = 15.0;
}

void MaxHealthBuff::SetFlatBonus( int32_t flatBonus )
{
    mFlatBonus = flatBonus;
}

int32_t MaxHealthBuff::GetFlatBonus()const
{
    return mFlatBonus;
}

void MaxHealthBuff::SetPercentBonus( double percentBonus )
{
    mPercentBonus = percentBonus;
}

double MaxHealthBuff::GetPercentBonus()const
{
    return mPercentBonus;
}


