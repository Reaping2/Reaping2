#ifndef INCLUDED_NETWORK_LIFECYCLE_SENDER_SYSTEM_H
#define INCLUDED_NETWORK_LIFECYCLE_SENDER_SYSTEM_H

#include "engine/system.h"
#include <enet/enet.h>
#include <map>
#include "messsage_holder.h"

namespace network {

class LifecycleSenderSystem: public engine::System
{
    DEFINE_SYSTEM_BASE(LifecycleSenderSystem)
    MessageHolder& mMessageHolder;
    ModelValue mLifecycleModel;
    ModelValue mHostModel;
public:
    LifecycleSenderSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
    void Host(std::string const& mode);
};

} // namespace network

#endif//INCLUDED_NETWORK_LIFECYCLE_SENDER_SYSTEM_H
