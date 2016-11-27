#ifndef INCLUDED_NETWORK_SERVER_SYSTEM_H
#define INCLUDED_NETWORK_SERVER_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include <enet/enet.h>
#include <map>
#include "messsage_holder.h"
#include "engine/frame_counter_system.h"
#include "platform/register.h"
#include "client_id_changed.h"
#include <atomic>
#include <thread>

namespace network {

class ServerSystem: public engine::System
{
    ENetAddress mAddress;
    ENetHost* mServer;
    typedef std::map<int32_t, ENetPeer*> Clients_t;
    std::mutex mClientsMutex;
    Clients_t mClients;
    int32_t mClientId;
    MessageHolder& mMessageHolder;
    AutoReg mOnFrameCounterEvent;
    void OnFrameCounterEvent( engine::FrameCounterEvent const& Evt );
    AutoReg mOnClientIdChanged;
    void OnClientIdChanged( ClientIdChangedEvent const& Evt );
    int32_t mSentMessagesSize;

    std::atomic<bool> mRunning;
    bool mThreaded;
    std::thread mThread;
    void UpdateThread();

    void SendMessages();

    void ReceiveMessages();

    void TransferOutgoingMessagesTo( MessageList::Messages_t& messages );

    void PublishIncomingMessages();

    AutoReg mOnPhaseChanged;
    void OnPhaseChanged( PhaseChangedEvent const& Evt );
public:
    DEFINE_SYSTEM_BASE( ServerSystem )
    ServerSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );

    void ClientDisconnect( ENetEvent& event );

    void ClientConnect( ENetEvent& event );

    void Receive( ENetEvent& event );

    void SetSenderId( MessageList::Messages_t& messages, ENetEvent& event );
};

} // namespace network

#endif//INCLUDED_NETWORK_SERVER_SYSTEM_H
