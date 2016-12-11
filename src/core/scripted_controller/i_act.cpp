#include "i_act.h"
namespace scriptedcontroller
{

IAct::IAct( int32_t Id )
    : mId(Id)
{

}


void IAct::Update( double Seconds )
{
    mDurationCurrent = std::min<double>( 0.0, mDurationCurrent - Seconds );
}

void IAct::Load( Json::Value const& setters )
{
    Json::GetDouble( setters["duration"], mDuration );
    Json::GetDouble( setters["duration_variance"], mDurationVariance );
}


void IAct::Start()
{
    mDurationCurrent = std::max<double>( 0,
        mDuration + 2 * mDurationVariance * (RandomGenerator::global()() % 100) / 100 - mDurationVariance );
}

void IAct::Stop()
{
    mDurationCurrent = mInterruptible ? 0 : mDurationCurrent;
}


bool IAct::IsInterruptible()
{
    return mInterruptible;
}


bool IAct::IsRunning()
{
    return mDurationCurrent > 0.0;
}

} // namespace scriptedcontroller

