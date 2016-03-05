#ifndef INCLUDED_NETWORK_LIFECYCLE_SENDER_SYSTEM_H
#define INCLUDED_NETWORK_LIFECYCLE_SENDER_SYSTEM_H

#include "engine/system.h"
#include "messsage_holder.h"
#include "core/scene.h"
#include "core/program_state.h"

namespace network {

class LifecycleSenderSystem: public engine::System
{
    DEFINE_SYSTEM_BASE(LifecycleSenderSystem)
    MessageHolder& mMessageHolder;
    ModelValue mLifecycleModel;
    ModelValue mHostModel;
    Scene& mScene;
	core::ProgramState& mProgramState;
public:
    LifecycleSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
    void Host();
};

} // namespace network

#endif//INCLUDED_NETWORK_LIFECYCLE_SENDER_SYSTEM_H
