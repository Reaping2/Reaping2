#include "platform/i_platform.h"
#include "network/team_switch_request_message.h"

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


void TeamSwitchRequestMessageSenderSystem::Update(double DeltaTime)
{
    MessageSenderSystem::Update(DeltaTime);
}

void TeamSwitchRequestMessageSenderSystem::OnTeamSwitchRequest( TeamSwitchRequestEvent const& event )
{
    std::cerr<< "Tean switch msg sender";
    if ( mProgramState.mMode == ProgramState::Client )
    {
        std::cerr << " (client) processing...";
        if ( mProgramState.mClientId != event.mClientId )
        {
            std::cerr << "error client != client\n";
            // ERROR: don't allow change other's team
            return;
        }
        std::auto_ptr<TeamSwitchRequestMessage> msg(new TeamSwitchRequestMessage);
        msg->mClientId = event.mClientId;
        mMessageHolder.AddOutgoingMessage( msg );
    }
    std::cerr<<"\n";
}

TeamSwitchRequestMessageHandlerSubSystem::TeamSwitchRequestMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{
}


void TeamSwitchRequestMessageHandlerSubSystem::Init()
{
}

void TeamSwitchRequestMessageHandlerSubSystem::Update(double DeltaTime)
{
    MessageHandlerSubSystem::Update(DeltaTime);
}

void TeamSwitchRequestMessageHandlerSubSystem::Execute(Message const& message)
{
    std::cerr<< "Tean switch msg handler";
    TeamSwitchRequestMessage const& msg=static_cast<TeamSwitchRequestMessage const&>(message);
    if ( mProgramState.mMode == ProgramState::Server )
    {
        std::cerr << " (server) processing...";
        TeamSwitchRequestEvent event;
        event.mClientId = msg.mClientId;
        EventServer<TeamSwitchRequestEvent>::Get().SendEvent( event );
    }
    std::cerr << "\n";
}

} // namespace network

