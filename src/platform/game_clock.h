#ifndef INCLUDED_PLATFORM_GAME_CLOCK_H
#define INCLUDED_PLATFORM_GAME_CLOCK_H
#include "platform/i_platform.h"
#include <vector>
#include <chrono>
#include "engine/system_suppressor.h"

namespace platform {

class Clock : public platform::Singleton<Clock>
{
public:
    // Elapsed time since game start in seconds
    static double Now();

    void UpdateElapsedTime();
    std::chrono::high_resolution_clock::time_point const& GetCurrentTime() const;
    std::chrono::high_resolution_clock::time_point const& GetStartTime() const;
private:
    friend class platform::Singleton<Clock>;
    Clock();
    std::chrono::high_resolution_clock::time_point mCurrentTime;
    std::chrono::high_resolution_clock::time_point mStartTime;
    double mNow = 0.0;
    AutoReg mOnCycleEvent;
    void OnCycleEvent( CycleEvent const& evt );
    AutoReg mOnSuppressEvent;
    void OnSuppressEvent( engine::SuppressEvent const& evt );
    std::chrono::high_resolution_clock::time_point mSuppressedTime;
    bool mSuppressed = false;
};
} // platform

#endif//INCLUDED_PLATFORM_GAME_CLOCK_H
