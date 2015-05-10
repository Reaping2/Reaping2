#ifndef INCLUDED_PLATFORM_FREQUENCY_TIMER_H
#define INCLUDED_PLATFORM_FREQUENCY_TIMER_H

#include "rstdint.h"

namespace platform {
class FrequencyTimer
{
    double mElapsedTime;
    double mFrequency;
    int32_t mElapsedTicks;
public:
    FrequencyTimer();
    bool Update( double DeltaTime );
    bool IsTime();
    int32_t GetElapsedTicks();
    void SetFrequency( double frequency );
    void Reset();
    virtual ~FrequencyTimer();
};

} // namespace platform

#endif//INCLUDED_PLATFORM_FREQUENCY_TIMER_H
