#include "platform/i_platform.h"
#include "network/client_system.h"
#include <boost/timer.hpp>
#include "core/program_state.h"
#include "messsage_holder.h"
#include <portable_oarchive.hpp>
#include <iosfwd>
#include "my_name_message.h"
#include "engine/engine.h"
#include "main/window.h"
#include "platform/settings.h"
namespace network {

ClientSystem::ClientSystem()
    : mAddress()
    , mClient( NULL )
    , mPeer( NULL )
    , mClientModel( "client", &RootModel::Get() )
    , mConnectModel( VoidFunc( this, &ClientSystem::Connect ), "connect", &mClientModel )
    , mMessageHolder( MessageHolder::Get() )
    , mProgramState( ProgramState::Get() )
    , mRunning( false )
    , mThreaded( false )
    , mWaitMillisecs( 10 )
{
    mOnPhaseChanged = EventServer<PhaseChangedEvent>::Get().Subscribe( boost::bind( &ClientSystem::OnPhaseChanged, this, _1 ) );
}

namespace {
class Timer_t
{
    ::boost::timer mMeasurer;
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

void ClientSystem::Init()
{
    if ( enet_initialize () != 0 )
    {
        L1 ( "An error occurred while initializing ENet.\n" );
    }
    atexit ( enet_deinitialize );
    Connect();
    mThreaded = Settings::Get().GetBool( "network.threaded", true );
    mRunning = true;
    mWaitMillisecs = Settings::Get().GetInt( "network.wait_millisec", 10 );
    if (mThreaded)
    {
        mThread = std::thread(boost::bind(&ClientSystem::UpdateThread,this));
    }
}

void ClientSystem::Update( double DeltaTime )
{
    PerfTimer.Log( "client update started" );

    if ( !mProgramState.mClientConnected )
    {
        PerfTimer.Log( "client not connected, client update ended" );
        return;
    }

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
    PerfTimer.Log( "client update ended" );
}

void ClientSystem::Connect()
{
    if ( mProgramState.mClientConnected )
    {
        L1( "Already connected, won't try it again!\n" );
        return; //Really wont try again
    }
    //        ENetAddress address2;
    //         address2.host = ENET_HOST_ANY;
    //         /* Bind the server to port 1234. */
    //         address2.port = 1236;
    mClient = enet_host_create ( NULL /* create a client host */,
                                 1 /* only allow 1 outgoing connection */,
                                 2 /* allow up 2 channels to be used, 0 and 1 */,
                                 0 ,
                                 0 );
    if ( mClient == NULL )
    {
        L1( "An error occurred while trying to create an ENet client host.\n" );
        exit ( EXIT_FAILURE );
    }

    ENetAddress address;
    ENetEvent event;
    /* Connect to some.server.net:1234. */
    address.port = 1234;
    enet_address_set_host ( & address, core::ProgramState::Get().mServerIp.c_str() ); //core::ProgramState::Get().mServerIp.c_str());
    /* Initiate the connection, allocating the two channels 0 and 1. */
    mPeer = enet_host_connect ( mClient, & address, 2, 0 );
    if ( mPeer == NULL )
    {
        L1( "No available peers for initiating an ENet connection.\n" );
        exit ( EXIT_FAILURE );
    }
    bool connectSuccess = false;
    for( size_t i = 0; i < 500; ++i )
    {
        if ( enet_host_service ( mClient, & event, 10 ) > 0 &&
             event.type == ENET_EVENT_TYPE_CONNECT )
        {
            connectSuccess = true;
            break;
        }
    }
    enet_host_flush( mClient );
    if( !connectSuccess )
    {
        L1( "Connection timed out.\n" );
        enet_peer_reset ( mPeer );
        engine::Engine::Get().GetSystem<engine::WindowSystem>()->Close();
    }
    else
    {
        mProgramState.mClientConnected = true;
        std::auto_ptr<MyNameMessage> msg( new MyNameMessage );
        msg->mName = core::ProgramState::Get().mClientName;
        msg->mControlledLocalPlayerId = 1; // TODO: change when multiple local players are allowed
        mMessageHolder.AddOutgoingMessage( std::auto_ptr<Message>( msg.release() ) );
    }
}
void ClientSystem::Receive( ENetEvent& event )
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
    mMessageHolder.GetIncomingMessages().TransferFrom( messages );

    /* Clean up the packet now that we're done using it. */
    enet_packet_destroy ( event.packet );
}

void ClientSystem::UpdateThread()
{
    while (mRunning)
    {
        //PerfTimer.Log( "client thread update started" );
        if (!mProgramState.mClientConnected)
        {
            //PerfTimer.Log( "client not connected, client update ended" );
            std::this_thread::sleep_for( std::chrono::milliseconds( mWaitMillisecs ) );
            continue;
        }

        ReceiveMessages();

        SendMessages();

        //PerfTimer.Log( "client thread update ended" );
    }
}

void ClientSystem::OnPhaseChanged( PhaseChangedEvent const& Evt )
{
    if (mThreaded && Evt.CurrentPhase == ProgramPhase::CloseSignal)
    {
        mRunning = false;
        mThread.join();
    }
}

void ClientSystem::PublishIncomingMessages()
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

void ClientSystem::TransferOutgoingMessagesTo( MessageList::Messages_t& messages )
{
    if (mThreaded)
    {
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

void ClientSystem::ReceiveMessages()
{
    ENetEvent event;
    while (enet_host_service( mClient, &event, 0 ) > 0)
    {
        //PerfTimer.Log("server enter");
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            Receive( event );
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
//            L1( "%s disconnected.\n", event.peer->data );
            mProgramState.mClientConnected = false;
//            L1( "\n\n\n\nLost connection please try reconnecting later! One Love!\n" );
            engine::Engine::Get().GetSystem<engine::WindowSystem>()->Close();
        }
    }
//    PerfTimer.Log( "client receive ended" );
    PublishIncomingMessages();
}

void ClientSystem::SendMessages()
{
    MessageList::Messages_t messages;
    TransferOutgoingMessagesTo( messages );
    if (!messages.empty())
    {

        std::ostringstream oss;
        eos::portable_oarchive oa( oss );
        oa & messages;
        std::string astr( oss.str() );
        ENetPacket* packet = enet_packet_create( astr.c_str(),
            astr.size(),
            ENET_PACKET_FLAG_RELIABLE );

        enet_peer_send( mPeer, 0, packet );
        enet_host_flush( mClient );
    }
}

} // namespace engine

