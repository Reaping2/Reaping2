#ifndef INCLUDED_NETWORK_WAITING_START_SYSTEM_H
#define INCLUDED_NETWORK_WAITING_START_SYSTEM_H

#include "engine/system.h"
#include "platform/register.h"
#include "platform/model_value.h"

namespace network {

using platform::AutoReg;
using platform::ModelValue;

class LoadClientlistEvent;

class WaitingStartSystem : public engine::System {
    ModelValue mWaitModel;
    AutoReg mOnLoadClientlistEvent;
    void OnLoadClientlistEvent( LoadClientlistEvent const & event );
public:    
    DEFINE_SYSTEM_BASE(WaitingStartSystem)
    WaitingStartSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
};

}

#endif
