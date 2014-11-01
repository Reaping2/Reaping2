#ifndef INCLUDED_PLATFORM_TIMER_H
#define INCLUDED_PLATFORM_TIMER_H

#include "singleton.h"
#include "register.h"
#include <boost/function.hpp>

namespace platform {
typedef boost::function<void()> TimerCallback;

namespace Timer {
#warning "namespace Timer is obsolete, and will be removed"
typedef ::platform::TimerCallback TimerCallback;
} // namespace Timer
class TimerServer : public Singleton<TimerServer>, public Registry
{
    friend class Singleton<TimerServer>;
    TimerServer();
    virtual void UpdateOne( void* RegistrationData, void* UpdateData );
    virtual void DeleteData( void* Data );
public:
    Registration AddTimer( TimerCallback Callback, double Interval );
    void Update( double Seconds );
};

} // namespace platform

#endif//INCLUDED_PLATFORM_TIMER_H
