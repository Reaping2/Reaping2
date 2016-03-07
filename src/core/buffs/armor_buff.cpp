#include "core/buffs/armor_buff.h"

ArmorBuff::ArmorBuff()
    : Buff()
    , mApplied( false )
    , mArmorMax( 50 )
{
    mSecsToEnd = 20.0;
}

void ArmorBuff::SetApplied( bool applied )
{
    mApplied = applied;
}

bool ArmorBuff::IsApplied()const
{
    return mApplied;
}

void ArmorBuff::SetArmorMax( int32_t armorMax )
{
    mArmorMax = armorMax;
}

int32_t ArmorBuff::GetArmorMax()const
{
    return mArmorMax;
}


