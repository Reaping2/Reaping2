#include "core/buffs/move_speed_buff.h"

MoveSpeedBuff::MoveSpeedBuff()
    : Buff()
    , mFlatBonus( 0 )
    , mPercentBonus( 10.0 )
    , mRooted( false )
{
    mSecsToEnd = 15.0;
}

void MoveSpeedBuff::SetFlatBonus( int32_t flatBonus )
{
    mFlatBonus = flatBonus;
}

int32_t MoveSpeedBuff::GetFlatBonus()const
{
    return mFlatBonus;
}

void MoveSpeedBuff::SetPercentBonus( double percentBonus )
{
    mPercentBonus = percentBonus;
}

double MoveSpeedBuff::GetPercentBonus()const
{
    return mPercentBonus;
}

void MoveSpeedBuff::SetRooted( bool rooted )
{
    mRooted = rooted;
}

bool MoveSpeedBuff::IsRooted() const
{
    return mRooted;
}


