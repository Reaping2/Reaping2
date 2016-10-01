#ifndef INCLUDED_CORE_PERF_TIMER_H
#define INCLUDED_CORE_PERF_TIMER_H

#include <boost/timer.hpp>
#include "platform/i_platform.h"

namespace perf {
    class Timer_t
    {
        boost::timer mMeasurer;
        double mPrevMeasurement;
    public:
        void Log( std::string const& Str = std::string() );
        Timer_t();
    };

} // namespace perf

#endif//INCLUDED_CORE_PERF_TIMER_H
