#ifndef INCLUDED_PLATFORM_TIMER_H
#define INCLUDED_PLATFORM_TIMER_H

class TimerServer;
struct Timer
{
    typedef boost::function<void()> TimerCallback;
private:
    friend class TimerServer;
    Timer( TimerCallback Callback, double Interval );
    void Update( double Seconds );

    double mRemaining;
    double mInterval;
    TimerCallback mCallback;
};

class TimerServer : public Singleton<TimerServer>, public Registry
{
    friend class Singleton<TimerServer>;
    TimerServer();
    virtual void UpdateOne( void* RegistrationData, void* UpdateData );
    virtual void DeleteData( void* Data );
public:
    Registration AddTimer( Timer::TimerCallback Callback, double Interval );
    void Update( double Seconds );
};

#endif//INCLUDED_PLATFORM_TIMER_H
