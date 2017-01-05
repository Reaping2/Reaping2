#include "game_clock.h"

namespace platform {

Clock::Clock()
{
    mStartTime = std::chrono::high_resolution_clock::now();
    mOnCycleEvent = EventServer<CycleEvent>::Get().Subscribe( boost::bind( &Clock::OnCycleEvent, this, _1 ) );
}

std::chrono::high_resolution_clock::time_point const& Clock::GetCurrentTime() const
{
    return Get().mCurrentTime;
}

void Clock::UpdateElapsedTime()
{
    mCurrentTime = std::chrono::high_resolution_clock::now();
    mNow = std::chrono::duration<double>( mCurrentTime - mStartTime ).count();
}

std::chrono::high_resolution_clock::time_point const& Clock::GetStartTime() const
{
    return Get().mStartTime;
}

double Clock::Now()
{
    return Get().mNow;
}

void Clock::OnCycleEvent( CycleEvent const& evt )
{
    UpdateElapsedTime();
}

} // platform
