#include "i_platform.h"

Registration TimerServer::AddTimer( Timer::TimerCallback Callback, double Interval)
{
	return Register(new Timer(Callback,Interval));
}

void TimerServer::Update( double Seconds )
{
	// expl. for the messy loop: callback might modify the registrations
	for(Registrations::iterator i=mRegistrations.begin(),e=mRegistrations.end(),n;(n=i,(i!=e)?(++n,true):false);i=n)
		((Timer*)(*i))->Update(Seconds);
}

TimerServer::TimerServer()
{

}

Timer::Timer( TimerCallback Callback,double Interval )
: mCallback(Callback)
, mInterval(Interval)
, mRemaining(Interval)
{
	assert(!mCallback.empty());
	assert(mInterval>0.);
}

void Timer::Update( double Seconds )
{
	mRemaining-=Seconds;
	if(mRemaining<=0.)
	{
		mRemaining+=mInterval;
		if(!mCallback.empty())
			mCallback();
		// DO NOTHING AFTER THIS, CALLBACK MIGHT DELETE THE TIMER!
	}
}
