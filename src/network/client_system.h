#ifndef INCLUDED_NETWORK_CLIENT_SYSTEM_H
#define INCLUDED_NETWORK_CLIENT_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include <enet/enet.h>
#include "platform/i_platform.h"
#include "messsage_holder.h"
#include "core/program_state.h"
using platform::ModelValue;
using core::ProgramState;
namespace network {

class ClientSystem: public engine::System
{
    DEFINE_SYSTEM_BASE(ClientSystem)
    ENetAddress mAddress;
    ENetHost * mClient;
    ENetPeer * mPeer;

    ModelValue mClientModel;
    ModelValue mConnectModel;
    MessageHolder& mMessageHolder;
    ProgramState& mProgramState;
public:
    ClientSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
    void Connect();
    void Receive(ENetEvent& event);
};

} // namespace network

#endif//INCLUDED_NETWORK_CLIENT_SYSTEM_H
