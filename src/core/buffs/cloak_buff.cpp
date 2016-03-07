#include "core/buffs/cloak_buff.h"

CloakBuff::CloakBuff()
    : Buff()
    , mApplied( false )
{
    mSecsToEnd = 5.0;
}

void CloakBuff::SetApplied( bool applied )
{
    mApplied = applied;
}

bool CloakBuff::IsApplied()const
{
    return mApplied;
}


