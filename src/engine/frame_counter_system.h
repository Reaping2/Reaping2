#ifndef INCLUDED_ENGINE_FRAME_COUNTER_SYSTEM_H
#define INCLUDED_ENGINE_FRAME_COUNTER_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class FrameCounterSystem: public System
{
    DEFINE_SYSTEM_BASE(FrameCounterSystem)
    uint32_t mFrames;
    double mStart;
    double mPrev;
    int32_t mFps;
public:
    ModelValue mFpsModel;
    int32_t const& GetFps();
    FrameCounterSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
};

} // namespace engine

#endif//INCLUDED_ENGINE_FRAME_COUNTER_SYSTEM_H
