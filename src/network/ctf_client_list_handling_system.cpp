#include "ctf_client_list_handling_system.h"
#include "platform/event.h"
#include "team_switch_request_event.h"
#include "ctf_client_datas_changed_event.h"
#include "client_ready_event.h"
#include <algorithm>

namespace network {


CtfClientListHandlingSystem::CtfClientListHandlingSystem()
{
    mOnTeamSwitchRequestEvent = 
        platform::EventServer<network::TeamSwitchRequestEvent>::Get().Subscribe( boost::bind( &CtfClientListHandlingSystem::OnTeamSwitchRequestEvent, this, _1 ) );
    mClientReadyEvent = 
        platform::EventServer<network::ClientReadyEvent>::Get().Subscribe( boost::bind( &CtfClientListHandlingSystem::OnClientReadyEvent, this, _1 ) );
}

void CtfClientListHandlingSystem::Init()
{
}

void CtfClientListHandlingSystem::Update( double DeltaTime )
{
}

void CtfClientListHandlingSystem::OnTeamSwitchRequestEvent( TeamSwitchRequestEvent const& event )
{
    ::ctf::ProgramState::ClientDatas_t& ctfClientDatas = ::ctf::ProgramState::Get().mClientDatas;
    std::vector<::ctf::ClientData>::iterator it = std::find_if( ctfClientDatas.begin(), ctfClientDatas.end(),
           boost::bind<bool>( []( ::ctf::ClientData const& d, int32_t clientId ){ return d.mClientId == clientId; }, _1, event.mClientId ) );
    if ( ctfClientDatas.end() == it )
    {
        // something went wrong if we end up here
        return;
    }
    Team::Type team = (*it).mTeam;

    // switch team
    if ( Team::Blue == team )
    {
	    (*it).mTeam = Team::Red;
    }
    else
    {
	    (*it).mTeam = Team::Blue;
    }
    // send out the new team setup
    CtfClientDatasChangedEvent clientDatasChangedEvent;
    clientDatasChangedEvent.mCtfClientDatas = ctfClientDatas;
    EventServer<CtfClientDatasChangedEvent>::Get().SendEvent(clientDatasChangedEvent);
}

void CtfClientListHandlingSystem::OnClientReadyEvent( ClientReadyEvent const& event )
{
    ::ctf::ProgramState::ClientDatas_t& ctfClientDatas = ::ctf::ProgramState::Get().mClientDatas;
    // if ready then add to 
    int reds = std::count_if( ctfClientDatas.begin(), ctfClientDatas.end(),
           []( ::ctf::ClientData const& d ){ return d.mTeam == Team::Red; } );
    int blues = std::count_if( ctfClientDatas.begin(), ctfClientDatas.end(),
           []( ::ctf::ClientData const& d ){ return d.mTeam == Team::Blue; } );
    ::ctf::ClientData data;
    data.mClientId = event.mClientId;
    data.mClientName = event.mClientName;
    // simple team distribution: add it to the smaller team
    if ( reds < blues )
    {
        data.mTeam = Team::Red;
    }
    else
    {
        data.mTeam = Team::Blue;
    }
    ctfClientDatas.push_back( data );
    // send out the new team setup
    CtfClientDatasChangedEvent evt;
    evt.mCtfClientDatas = ctfClientDatas;
    EventServer<CtfClientDatasChangedEvent>::Get().SendEvent(evt);
}

}
