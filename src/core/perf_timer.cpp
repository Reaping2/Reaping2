#include "perf_timer.h"
namespace perf {

    void Timer_t::Log( std::string const& Str /*= std::string() */ )
    {
        double Now = mMeasurer.elapsed();
        L2( "Timer - %s: %f %f\n", Str.c_str(), Now, Now - mPrevMeasurement );
        mPrevMeasurement = Now;
    }

    Timer_t::Timer_t() : mMeasurer(), mPrevMeasurement( mMeasurer.elapsed() )
    {
        Log( "timer init" );
    }

} // namespace perf
