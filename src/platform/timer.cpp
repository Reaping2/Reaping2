#include "i_platform.h"

Registration TimerServer::AddTimer( Timer::TimerCallback Callback, double Interval)
{
	return Register(new Timer(Callback,Interval));
}

void TimerServer::Update( double Seconds )
{
	Registry::Update(&Seconds);
}

TimerServer::TimerServer()
{

}

void TimerServer::UpdateOne( void* RegistrationData, void* UpdateData )
{
	double Seconds=*(double*)UpdateData;
	Timer* T=(Timer*)RegistrationData;
	T->Update(Seconds);
}

void TimerServer::DeleteData( void* Data )
{
	delete (Timer*)Data;
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
