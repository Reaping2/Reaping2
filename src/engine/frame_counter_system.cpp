#include "platform/i_platform.h"
#include "engine/frame_counter_system.h"
#include "platform/model_value.h"
namespace engine {

FrameCounterSystem::FrameCounterSystem()
        : mFrames( 0 )
        , mStart( 0.0 )
        , mPrev( 0.0 ) 
        , mFps( 0 )
        , mFpsModel(GetFps(),"fps",&RootModel::Get())
{
}


void FrameCounterSystem::Init()
{
    mStart=glfwGetTime();
    mPrev=mStart;

}

void FrameCounterSystem::Update(double DeltaTime)
{
    ++mFrames;
    double const Now = glfwGetTime();
    double const Diff = Now - mPrev;
    if( Diff >= 2.0 )
    {
        L1( "FPS: %f\n", mFrames / Diff );
        mFps=mFrames / Diff;
        mPrev = Now;
        mFrames = 0;
    }
}

int32_t const& FrameCounterSystem::GetFps()
{
    return mFps;
}

} // namespace engine

