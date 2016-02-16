#ifndef INCLUDED_NETWORK_SERVER_SYSTEM_H
#define INCLUDED_NETWORK_SERVER_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include <enet/enet.h>
#include <map>
#include "messsage_holder.h"
#include "engine/frame_counter_system.h"
#include "platform/register.h"

namespace network {

class ServerSystem: public engine::System
{
    ENetAddress mAddress;
    ENetHost * mServer;
    typedef std::map<int32_t, ENetPeer*> Clients_t;
    Clients_t mClients;
    int32_t mClientId;
    MessageHolder& mMessageHolder;
    AutoReg mOnFrameCounterEvent;
    void OnFrameCounterEvent( engine::FrameCounterEvent const& Evt );
    int32_t mSentMessagesSize;
public:
    DEFINE_SYSTEM_BASE(ServerSystem)
    ServerSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );

    void ClientDisconnect(ENetEvent& event);

    void ClientConnect(ENetEvent& event);

    void Receive(ENetEvent& event);

    void SetSenderId(MessageList& msglist, ENetEvent &event);
};

} // namespace network

#endif//INCLUDED_NETWORK_SERVER_SYSTEM_H
