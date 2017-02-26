#ifndef INCLUDED_PLATFORM_FREQUENCY_TIMER_H
#define INCLUDED_PLATFORM_FREQUENCY_TIMER_H

#include "rstdint.h"
#include "platform/export.h"
#include <boost/serialization/serialization.hpp>
#include <boost/ptr_container/serialize_ptr_map.hpp>
#include "json/json.h"

namespace platform {
class FrequencyTimer
{
    double mElapsedTime;
    double mFrequency;
    int32_t mElapsedTicks;
public:
    FrequencyTimer();
    bool Update( double DeltaTime );
    bool IsTime() const;
    int32_t GetElapsedTicks();
    void SetFrequency( double frequency );
    double GetFrequency() const;
    void Load( Json::Value const& frequencyValue );
    void Reset();
    virtual ~FrequencyTimer();
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void platform::FrequencyTimer::serialize( Archive& ar, const unsigned int version )
{
    ar& mElapsedTime;
    ar& mFrequency;
    ar& mElapsedTicks;
}

} // namespace platform

#endif//INCLUDED_PLATFORM_FREQUENCY_TIMER_H
