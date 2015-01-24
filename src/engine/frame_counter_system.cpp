#include "platform/i_platform.h"
#include "engine/frame_counter_system.h"
namespace engine {

FrameCounterSystem::FrameCounterSystem()
        : mFrames( 0 )
        , mStart( 0.0 )
        , mPrev( 0.0 ) 
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
        mPrev = Now;
        mFrames = 0;
    }
}

} // namespace engine

