#include "core/buffs/accuracy_buff.h"

AccuracyBuff::AccuracyBuff()
    : Buff()
    , mFlatBonus( 40 )
    , mPercentBonus( 0 )
{
    mSecsToEnd = 20.0;
}

void AccuracyBuff::SetFlatBonus( int32_t flatBonus )
{
    mFlatBonus = flatBonus;
}

int32_t AccuracyBuff::GetFlatBonus()const
{
    return mFlatBonus;
}

void AccuracyBuff::SetPercentBonus( double percentBonus )
{
    mPercentBonus = percentBonus;
}

double AccuracyBuff::GetPercentBonus()const
{
    return mPercentBonus;
}


