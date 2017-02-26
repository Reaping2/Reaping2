#include "network/message_sender_system.h"
namespace network {

MessageSenderSystem::MessageSenderSystem()
    : mFrequencyTimer()
    , mMessageHolder( MessageHolder::Get() )
    , mScene( Scene::Get() )
    , mProgramState( ProgramState::Get() )
{
    mIsClient = mProgramState.mMode == ProgramState::Client;
    mIsServer = mProgramState.mMode == ProgramState::Server;
}

void MessageSenderSystem::Update( double DeltaTime )
{
    mFrequencyTimer.Update( DeltaTime );
}

bool MessageSenderSystem::IsTime() const
{
    return mFrequencyTimer.IsTime();
}

void MessageSenderSystem::SetFrequency( double frequency )
{
    mFrequencyTimer.SetFrequency( frequency );
}

void MessageSenderSystem::Init()
{
    mFrequencyTimer.Reset();
}

bool MessageSenderSystem::IsClient() const
{
    return mIsClient;
}

bool MessageSenderSystem::IsServer() const
{
    return mIsServer;
}

} // namespace network

