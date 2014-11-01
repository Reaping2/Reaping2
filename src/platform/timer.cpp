#include "timer.h"

#include <boost/assert.hpp>

namespace platform {
namespace {
namespace detail {

struct Timer
{
    Timer( TimerCallback Callback, double Interval );
    Timer& operator = ( Timer const& );
    void Update( double Seconds );

    double mRemaining;
    double mInterval;
    TimerCallback mCallback;
};

Timer::Timer( TimerCallback Callback, double Interval )
    : mCallback( Callback )
    , mInterval( Interval )
    , mRemaining( Interval )
{
    assert( !mCallback.empty() );
    assert( mInterval > 0. );
}

void Timer::Update( double Seconds )
{
    mRemaining -= Seconds;
    if( mRemaining <= 0. )
    {
        mRemaining += mInterval;
        if( !mCallback.empty() )
        {
            mCallback();
        }
        // DO NOTHING AFTER THIS, CALLBACK MIGHT DELETE THE TIMER!
    }
}

} // namespace detail
} // namespace anonymous

Registration TimerServer::AddTimer( TimerCallback Callback, double Interval )
{
    return Register( new detail::Timer( Callback, Interval ) );
}

void TimerServer::Update( double Seconds )
{
    Registry::Update( &Seconds );
}

TimerServer::TimerServer()
{

}

void TimerServer::UpdateOne( void* RegistrationData, void* UpdateData )
{
    double Seconds = *static_cast< double* >( UpdateData );
    detail::Timer* T = static_cast< detail::Timer* >( RegistrationData );
    T->Update( Seconds );
}

void TimerServer::DeleteData( void* Data )
{
    delete static_cast< detail::Timer* >( Data );
}

} // namespace platform

