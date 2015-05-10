#include "platform/i_platform.h"
#include "core/buffs/buff.h"

Buff::Buff()
    : mSecsToEnd(0.0)
{
}

double Buff::GetSecsToEnd()
{
    return mSecsToEnd;
}

void Buff::SetSecsToEnd(double secsToEnd)
{
    mSecsToEnd=secsToEnd;
}
Buff::~Buff()
{
}

DefaultBuff::DefaultBuff()
    : Buff()
{
}

