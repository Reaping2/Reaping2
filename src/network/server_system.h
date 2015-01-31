#ifndef INCLUDED_NETWORK_SERVER_SYSTEM_H
#define INCLUDED_NETWORK_SERVER_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include <enet/enet.h>

namespace network {

class ServerSystem: public engine::System
{
    DEFINE_SYSTEM_BASE(ServerSystem)
    ENetAddress mAddress;
    ENetHost * mServer;
public:
    ServerSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
};

} // namespace network

#endif//INCLUDED_NETWORK_SERVER_SYSTEM_H
