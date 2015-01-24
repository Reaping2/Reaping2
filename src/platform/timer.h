#ifndef INCLUDED_PLATFORM_TIMER_H
#define INCLUDED_PLATFORM_TIMER_H

#include "singleton.h"
#include "register.h"
#include <boost/function.hpp>
#include "engine/system.h"

namespace engine {
typedef boost::function<void()> TimerCallback;
using platform::Registry;
using platform::Registration;
class TimerServerSystem : public System, public Registry
{
    virtual void UpdateOne( void* RegistrationData, void* UpdateData );
    virtual void DeleteData( void* Data );
public:
    DEFINE_SYSTEM_BASE(TimerServerSystem)
    Registration AddTimer( TimerCallback Callback, double Interval );
    virtual void Init();
    virtual void Update( double DeltaTime );
    TimerServerSystem();
};

} // namespace platform

#endif//INCLUDED_PLATFORM_TIMER_H
