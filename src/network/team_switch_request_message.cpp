#include "platform/i_platform.h"
#include "network/team_switch_request_message.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace network {

TeamSwitchRequestMessageSenderSystem::TeamSwitchRequestMessageSenderSystem()
    : MessageSenderSystem()
{
    mOnTeamSwitchRequestEvent = platform::EventServer<TeamSwitchRequestEvent>::Get().Subscribe( boost::bind( &TeamSwitchRequestMessageSenderSystem::OnTeamSwitchRequest, this, _1 ) );
}


void TeamSwitchRequestMessageSenderSystem::Init()
{
    MessageSenderSystem::Init();
}


void TeamSwitchRequestMessageSenderSystem::Update( double DeltaTime )
{
    MessageSenderSystem::Update( DeltaTime );
}

void TeamSwitchRequestMessageSenderSystem::OnTeamSwitchRequest( TeamSwitchRequestEvent const& event )
{
    if ( mProgramState.mMode == ProgramState::Client )
    {
        if ( mProgramState.mClientId != event.mClientId )
        {
            // filtering: don't allow changing someone else's team
            return;
        }
        std::auto_ptr<TeamSwitchRequestMessage> msg( new TeamSwitchRequestMessage );
        msg->mClientId = event.mClientId;
        mMessageHolder.AddOutgoingMessage( msg );
    }
}

TeamSwitchRequestMessageHandlerSubSystem::TeamSwitchRequestMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void TeamSwitchRequestMessageHandlerSubSystem::Init()
{
}

void TeamSwitchRequestMessageHandlerSubSystem::Update( double DeltaTime )
{
    MessageHandlerSubSystem::Update( DeltaTime );
}

void TeamSwitchRequestMessageHandlerSubSystem::Execute( Message const& message )
{
    TeamSwitchRequestMessage const& msg = static_cast<TeamSwitchRequestMessage const&>( message );
    if ( mProgramState.mMode == ProgramState::Server )
    {
        TeamSwitchRequestEvent event;
        event.mClientId = msg.mClientId;
        EventServer<TeamSwitchRequestEvent>::Get().SendEvent( event );
    }
}

} // namespace network
REAPING2_CLASS_EXPORT_IMPLEMENT( network__TeamSwitchRequestMessage, network::TeamSwitchRequestMessage )

