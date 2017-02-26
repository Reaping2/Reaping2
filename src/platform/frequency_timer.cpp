#include "platform/frequency_timer.h"
#include "jsonreader.h"
namespace platform {

FrequencyTimer::FrequencyTimer()
    : mElapsedTime( 0.0 )
    , mFrequency( 0.0 )
    , mElapsedTicks( 0 )
{
}

bool FrequencyTimer::Update( double DeltaTime )
{
    if ( mFrequency == 0.0 )
    {
        return true;
    }

    mElapsedTicks = 0;
    mElapsedTime += DeltaTime * 1000;
    if( mElapsedTime >= mFrequency )
    {
        mElapsedTicks = ( int( mElapsedTime ) / int( mFrequency ) );
        mElapsedTime = ( int( mElapsedTime ) % int( mFrequency ) );
    }
    return mElapsedTicks > 0;
}

int32_t FrequencyTimer::GetElapsedTicks()
{
    return mElapsedTicks;
}

bool FrequencyTimer::IsTime() const
{
    return mElapsedTicks > 0 || mFrequency == 0.0;
}

void FrequencyTimer::SetFrequency( double frequency )
{
    mFrequency = frequency * 1000;
}


double FrequencyTimer::GetFrequency() const
{
    return mFrequency / 1000.;
}

void FrequencyTimer::Load( Json::Value const& frequencyValue )
{
    double frequency = 1.0;
    Json::GetDouble( frequencyValue, frequency );
    SetFrequency( frequency );
}

void FrequencyTimer::Reset()
{
    mElapsedTime = 0.0;
    mElapsedTicks = 0;
}

FrequencyTimer::~FrequencyTimer()
{

}
} // namespace platform
