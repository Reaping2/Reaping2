#ifndef INCLUDED_NETWORK_SERVER_SYSTEM_H
#define INCLUDED_NETWORK_SERVER_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include <enet/enet.h>
#include <map>
#include "messsage_holder.h"

namespace network {

class ServerSystem: public engine::System
{
    DEFINE_SYSTEM_BASE(ServerSystem)
    ENetAddress mAddress;
    ENetHost * mServer;
    typedef std::map<int32_t, ENetPeer*> Clients_t;
    Clients_t mClients;
    int32_t mClientId;
    MessageHolder& mMessageHolder;
public:
    ServerSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );

    void Receive(ENetEvent& event);

};

} // namespace network

#endif//INCLUDED_NETWORK_SERVER_SYSTEM_H
