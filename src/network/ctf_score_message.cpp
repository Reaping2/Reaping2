#include "platform/i_platform.h"
#include "network/ctf_score_message.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {
namespace ctf {

CtfScoreMessageSenderSystem::CtfScoreMessageSenderSystem()
    : MessageSenderSystem()
    , mCtfProgramState( ::ctf::ProgramState::Get() )
{
    SetFrequency( 300 );
}


void CtfScoreMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
}


void CtfScoreMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
    if ( !IsTime() )
    {
        return;
    }
    std::auto_ptr<CtfScoreMessage> ctfScoreMessage( new CtfScoreMessage );
    ctfScoreMessage->mBlueScore = mCtfProgramState.mBlueScore;
    ctfScoreMessage->mRedScore = mCtfProgramState.mRedScore;
    mMessageHolder.AddOutgoingMessage( ctfScoreMessage );
}

CtfScoreMessageHandlerSubSystem::CtfScoreMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
    , mCtfProgramState( ::ctf::ProgramState::Get() )
{
}


void CtfScoreMessageHandlerSubSystem::Init()
{
}

void CtfScoreMessageHandlerSubSystem::Update( double DeltaTime )
{
    MessageHandlerSubSystem::Update( DeltaTime );
}

void CtfScoreMessageHandlerSubSystem::Execute( Message const& message )
{
    CtfScoreMessage const& msg = static_cast<CtfScoreMessage const&>( message );
    mCtfProgramState.mBlueScore = msg.mBlueScore;
    mCtfProgramState.mRedScore = msg.mRedScore;
}

} // namespace ctf
} // namespace network


REAPING2_CLASS_EXPORT_IMPLEMENT( network__ctf__CtfScoreMessage, network::ctf::CtfScoreMessage );
