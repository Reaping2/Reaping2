#include "network/ping_message.h"
#include "GLFW/glfw3.h"
#include "ui/ui.h"
#include <boost/lambda/bind.hpp>
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>
#include "platform/game_clock.h"

namespace network {

PingMessageSenderSystem::PingMessageSenderSystem()
    : MessageSenderSystem()
{

}

void PingMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
    SetFrequency( 200 );
}

void PingMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
    if ( !IsTime() )
    {
        return;
    }
    std::auto_ptr<PingMessage> pingMsg( new PingMessage );
    pingMsg->mClientId = mProgramState.mClientId;
    pingMsg->mCurrentTime = platform::Clock::Now();
    mMessageHolder.AddOutgoingMessage( pingMsg );
}

PingMessageHandlerSubSystem::PingMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
    , mPing( 0.0 )
    , mPingModel( platform::GetIntFunc( this, &PingMessageHandlerSubSystem::GetPing ), "ping", &RootModel::Get() )
{
    if( mProgramState.mMode != ProgramState::Client )
    {
    }
}

void PingMessageHandlerSubSystem::Init()
{

}

void PingMessageHandlerSubSystem::Execute( Message const& message )
{
    PingMessage const& msg = static_cast<PingMessage const&>( message );
    if( mProgramState.mMode == ProgramState::Client )
    {
        if ( msg.mClientId == mProgramState.mClientId )
        {
            mPing = ( platform::Clock::Now() - msg.mCurrentTime ) * 1000;
            L2( "current ping: %d\n", mPing );
        }
    }
    else if ( mProgramState.mMode == ProgramState::Server )
    {
        std::auto_ptr<PingMessage> pingMessage( new PingMessage( msg ) );
        mMessageHolder.AddOutgoingMessage( pingMessage );
    }
}

int32_t PingMessageHandlerSubSystem::GetPing()
{
    return mPing;
}

} // namespace engine


REAPING2_CLASS_EXPORT_IMPLEMENT( network__PingMessage, network::PingMessage );
