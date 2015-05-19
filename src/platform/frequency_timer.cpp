#include "platform/frequency_timer.h"
namespace platform {

FrequencyTimer::FrequencyTimer()
    : mFrequency(0.0)
    , mElapsedTime(0.0)
    , mElapsedTicks(0)
{
}

bool FrequencyTimer::Update(double DeltaTime)
{
    if (mFrequency==0.0)
    {
        return true;
    }

    mElapsedTicks=0;
    mElapsedTime+=DeltaTime*1000;
    if(mElapsedTime>=mFrequency)
    {
        mElapsedTicks=(int(mElapsedTime)/int(mFrequency));
        mElapsedTime=(int(mElapsedTime)%int(mFrequency));
    }
    return mElapsedTicks>0;
}

int32_t FrequencyTimer::GetElapsedTicks()
{
    return mElapsedTicks;
}

bool FrequencyTimer::IsTime()
{
    return mElapsedTicks>0||mFrequency==0.0;
}

void FrequencyTimer::SetFrequency(double frequency)
{
    mFrequency=frequency;
}

void FrequencyTimer::Reset()
{
    mElapsedTime=0.0;
    mElapsedTicks=0;
}

FrequencyTimer::~FrequencyTimer()
{

}
} // namespace platform
