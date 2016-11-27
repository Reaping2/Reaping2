#include "platform/i_platform.h"
#include "network/server_system.h"
#include <boost/timer.hpp>
#include <portable_iarchive.hpp>
#include "my_name_message.h"
#include "client_system.h"
#include "engine/frame_counter_system.h"
#include "core/program_state.h"
#include "engine/connection_event.h"
#include "platform/settings.h"
namespace network {

ServerSystem::ServerSystem()
    : mClientId( 0 )
    , mMessageHolder( MessageHolder::Get() )
    , mSentMessagesSize( 0 )
    , mServer( NULL )
    , mAddress()
    , mRunning( false )
    , mThreaded( false )
{
    mOnPhaseChanged = EventServer<PhaseChangedEvent>::Get().Subscribe( boost::bind( &ServerSystem::OnPhaseChanged, this, _1 ) );
}

namespace {
class Timer_t
{
    boost::timer mMeasurer;
    double mPrevMeasurement;
public:
    void Log( std::string const& Str = std::string() )
    {
        double Now = mMeasurer.elapsed();
        L2( "Timer - %s: %f %f\n", Str.c_str(), Now, Now - mPrevMeasurement );
        mPrevMeasurement = Now;
    }
    Timer_t(): mMeasurer(), mPrevMeasurement( mMeasurer.elapsed() )
    {
        Log( "timer init" );
    }
} PerfTimer;

}

void ServerSystem::Init()
{
    if ( enet_initialize () != 0 )
    {
        L1 ( "An error occurred while initializing ENet.\n" );
    }
    atexit ( enet_deinitialize );
    mOnFrameCounterEvent = EventServer<engine::FrameCounterEvent>::Get().Subscribe( boost::bind( &ServerSystem::OnFrameCounterEvent, this, _1 ) );
    mOnClientIdChanged = EventServer<ClientIdChangedEvent>::Get().Subscribe( boost::bind( &ServerSystem::OnClientIdChanged, this, _1 ) );

    /* Bind the server to the default localhost.     */
    /* A specific host address can be specified by   */
    /* enet_address_set_host (& address, "x.x.x.x"); */
    mAddress.host = ENET_HOST_ANY;
    /* Bind the server to port 1234. */
    mAddress.port = 1234;
    mServer = enet_host_create ( & mAddress /* the address to bind the server host to */,
                                 32      /* allow up to 32 clients and/or outgoing connections */,
                                 2      /* allow up to 2 channels to be used, 0 and 1 */,
                                 0      /* assume any amount of incoming bandwidth */,
                                 0      /* assume any amount of outgoing bandwidth */ );
    if ( mServer == NULL )
    {
        L1 ( "An error occurred while trying to create an ENet server host.\n" );
    }
    mThreaded = Settings::Get().GetBool( "network.threaded", true );
    if (mThreaded)
    {
        mRunning = true;
        mThread = std::thread( boost::bind( &ServerSystem::UpdateThread, this ) );
    }
}

void ServerSystem::Update( double DeltaTime )
{
    PerfTimer.Log( "server update started" );

    if (mThreaded)
    {
        std::lock_guard<std::mutex> lck( mMessageHolder.GetOutgoingMessages().GetMutex() );
        mMessageHolder.GetOutgoingMessages().Publish();
        mMessageHolder.GetOutgoingMessages().GetCV().notify_all();
    }
    else
    {
        ReceiveMessages();
        mMessageHolder.GetOutgoingMessages().Publish();
        SendMessages();
    }

    PerfTimer.Log( "server update ended" );

}

void ServerSystem::Receive( ENetEvent& event )
{
    //     L1 ("A packet of length %u containing %s was received from %s on channel %u.\n",
    //         event.packet -> dataLength,
    //         event.packet -> data,
    //         event.peer -> data,
    //         event.channelID);
    std::istringstream iss( std::string( ( char* )( event.packet->data ), event.packet->dataLength ) );
    eos::portable_iarchive ia( iss );

    MessageList::Messages_t messages;
    ia >> messages;
    SetSenderId( messages, event );
    mMessageHolder.GetIncomingMessages().TransferFrom( messages );

    /* Clean up the packet now that we're done using it. */
    enet_packet_destroy ( event.packet );
}

void ServerSystem::SetSenderId( MessageList::Messages_t& messages, ENetEvent& event )
{
    const int senderId = *(static_cast<int*>(event.peer->data));
    for (auto& message : messages)
    {
        message.mSenderId = senderId;
    }
}

void ServerSystem::ClientConnect( ENetEvent& event )
{
//     L1 ( "A new client connected from %x:%u.\n",
//          event.peer -> address.host,
//          event.peer -> address.port );
    /* Store any relevant client information here. */
    std::lock_guard<std::mutex> lck( mClientsMutex );
    int32_t clientId = mClientId++;
    event.peer -> data = static_cast<void*>( new int32_t( clientId ) );
    mClients[clientId] = event.peer;
}

void ServerSystem::OnFrameCounterEvent( engine::FrameCounterEvent const& Evt )
{
    L1( "sent messages size (approx): %f\n", mSentMessagesSize / Evt.mDiff );
    mSentMessagesSize = 0;
}

void ServerSystem::ClientDisconnect( ENetEvent& event )
{
    std::lock_guard<std::mutex> lck( mClientsMutex );
    int32_t clientId = *static_cast<int32_t*>( event.peer -> data );
    Opt<core::ClientData> clientData( core::ProgramState::Get().FindClientDataByClientId( clientId ) );
//     L1 ( "client disconnected: %x:%u. client id: %d, name: %s\n",
//          event.peer -> address.host,
//          event.peer -> address.port,
//          clientId,
//          clientData.IsValid() ? clientData->mClientName.c_str() : "_no_name_" );
    /* Reset the peer's client information. */
    delete static_cast<int32_t*>( event.peer->data );
    event.peer -> data = NULL;
    if ( clientData.IsValid() )
    {
        // could be invalid if client tried to connect too early, and the system had it as connected
        clientData->mConnected = false;
    }
    mClients.erase( clientId );
    EventServer<engine::ConnectionEvent>::Get().SendEvent( engine::ConnectionEvent( clientId, engine::ConnectionEvent::Disconnected ) );
}

void ServerSystem::OnClientIdChanged( ClientIdChangedEvent const& Evt )
{
    std::lock_guard<std::mutex> lck( mClientsMutex );
    auto currPeer = mClients.find( Evt.mCurrClientId );
    if ( currPeer == mClients.end() )
    {
        // client connect should happen before client id change
        BOOST_ASSERT( currPeer != mClients.end() );
        return;
    }
    // client should be removed at this time (clientDisconnet should happen before client can reconnect)
    BOOST_ASSERT( mClients.find( Evt.mNewClientId ) == mClients.end() );
    delete static_cast<int32_t*>( currPeer->second->data );
    currPeer->second -> data = static_cast<void*>( new int32_t( Evt.mNewClientId ) );
    mClients[Evt.mNewClientId] = currPeer->second;
    mClients.erase( Evt.mCurrClientId );
}

void ServerSystem::UpdateThread()
{
    while (mRunning)
    {
        ReceiveMessages();
        SendMessages();
    }
}

void ServerSystem::SendMessages()
{
    MessageList::Messages_t messages;
    TransferOutgoingMessagesTo( messages );
    if (!messages.empty())
    {
        std::ostringstream oss;
        eos::portable_oarchive oa( oss );
        oa & messages;
        std::string astr( oss.str() );
        // L1("server sends - %s:\n",astr.c_str());
        ENetPacket* packet = enet_packet_create( astr.c_str(),
            astr.size(),
            ENET_PACKET_FLAG_RELIABLE );

        enet_host_broadcast( mServer, 0, packet );
        enet_host_flush( mServer );
    }
}

void ServerSystem::ReceiveMessages()
{
    ENetEvent event;
    while (enet_host_service( mServer, &event, 0 ) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            ClientConnect( event );
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            Receive( event );
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            ClientDisconnect( event );
            break;
        }
    }
    PublishIncomingMessages();
}

void ServerSystem::TransferOutgoingMessagesTo( MessageList::Messages_t& messages )
{
    if (mThreaded)
    {
        static const int32_t mWaitMillisecs = Settings::Get().GetInt( "network.wait_millisec", 10 );
        std::unique_lock<std::mutex> ulck( mMessageHolder.GetOutgoingMessages().GetMutex() );
        if (!mMessageHolder.GetOutgoingMessages().HasPublishedMessages())
        {
            mMessageHolder.GetOutgoingMessages().GetCV().wait_for( ulck, std::chrono::milliseconds( mWaitMillisecs ) );
        }
        mMessageHolder.GetOutgoingMessages().TransferPublishedMessagesTo( messages );
    }
    else
    {
        mMessageHolder.GetOutgoingMessages().TransferPublishedMessagesTo( messages );
    }
}

void ServerSystem::PublishIncomingMessages()
{
    if (mThreaded)
    {
        std::lock_guard<std::mutex> lck( mMessageHolder.GetIncomingMessages().GetMutex() );
        mMessageHolder.GetIncomingMessages().Publish();
    }
    else
    {
        mMessageHolder.GetIncomingMessages().Publish();
    }
}

void ServerSystem::OnPhaseChanged( PhaseChangedEvent const& Evt )
{
    if (mThreaded && Evt.CurrentPhase == ProgramPhase::CloseSignal)
    {
        mRunning = false;
        mThread.join();
    }
}

} // namespace engine

