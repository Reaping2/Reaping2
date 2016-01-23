#include "ctf_client_list_system.h"
#include "platform/event.h"
#include "ctf_client_datas_message.h"
#include "ctf_client_datas_changed_event.h"
#include <algorithm>

namespace network {

using platform::RootModel;

CtfClientListSystem::CtfClientListSystem()
    : mCTFModel("ctf_client_list", &RootModel::Get())
    , mRedTeamModel( "red", &mCTFModel )
    , mBlueTeamModel( "blue", &mCTFModel )
    , mBlueNamesModel( (ModelValue::get_string_vec_t) boost::bind( &CtfClientListSystem::BlueNames, this) , "names", &mBlueTeamModel)
    , mRedNamesModel( (ModelValue::get_string_vec_t) boost::bind( &CtfClientListSystem::RedNames, this) , "names", &mRedTeamModel)
    , mSwitchModel( StringFunc( this, &CtfClientListSystem::SwitchTeam ), "switch", &mCTFModel )
{
	mOnClientListChanged = platform::EventServer<network::ClientListChangedEvent>::Get().Subscribe( boost::bind( &CtfClientListSystem::OnClientListChanged, this, _1 ) );
}

void CtfClientListSystem::Init()
{
}

void CtfClientListSystem::Update( double DeltaTime )
{
}

std::vector<std::string> CtfClientListSystem::BlueNames()
{
	return mBlueNames;
}

std::vector<std::string> CtfClientListSystem::RedNames()
{
    return mRedNames;
}

void CtfClientListSystem::RemoveAll( core::ProgramState::ClientDatas_t & from , CtfClientListSystem::PlayerClientDataMap const & what )
{
    for ( PlayerClientDataMap::const_iterator playerIt = what.begin(); playerIt != what.end(); ++playerIt )
    {
        for ( core::ProgramState::ClientDatas_t::iterator it = from.begin(); it != from.end(); ++it )
        {
            if ( (*it).mClientId == playerIt->second.mClientId )
            {
                from.erase( it );
                break;
            }
        }
    }
}

void CtfClientListSystem::OnClientListChanged( ClientListChangedEvent const& event )
{
    core::ProgramState::ClientDatas_t const & clients = event.mClientList;
    core::ProgramState::ClientDatas_t readyClients(clients.size());
    auto it = std::copy_if( clients.begin(), clients.end(), readyClients.begin(), [] ( core::ClientData const & d ) { return d.mReady; } );
    readyClients.resize(std::distance(readyClients.begin(), it));
    // remove all blues and reds from clients -> new clients
    RemoveAll( readyClients, mPlayerToClientData );
    // simple distribute: add to the smaller team
    for ( size_t i = 0; i < readyClients.size(); ++i )
    {
        std::string const & clientName = readyClients[i].mClientName;
        int32_t clientId = readyClients[i].mClientId;
        if ( mBlueNames.size() < mRedNames.size() )
        {
            mBlueNames.push_back(clientName);
            mPlayerToClientData[clientName].mTeam = Team::Blue;
            mPlayerToClientData[clientName].mClientId = clientId;
        }
        else
        {
            mRedNames.push_back(clientName);
            mPlayerToClientData[clientName].mTeam = Team::Red;
            mPlayerToClientData[clientName].mClientId = clientId;
        }
    }
    // notify the server about the team setup
    CtfClientDatasChangedEvent ctfevent;
    ctfevent.mCtfClientDatas = createClientDatas();
    EventServer<CtfClientDatasChangedEvent>::Get().SendEvent(ctfevent);
}

void CtfClientListSystem::SwitchTeam( std::string const & player )
{
    ::ctf::ClientData & clientData = mPlayerToClientData[player];
    if ( Team::Blue == clientData.mTeam )
    {
        mBlueNames.erase( std::find(mBlueNames.begin(), mBlueNames.end(), player) );
        clientData.mTeam = Team::Red;
    }
    else
    {
        mRedNames.erase( std::find(mRedNames.begin(), mRedNames.end(), player) );
        clientData.mTeam = Team::Blue;
    }
    // notify the server about the team setup
    CtfClientDatasChangedEvent ctfevent;
    ctfevent.mCtfClientDatas = createClientDatas();
    EventServer<CtfClientDatasChangedEvent>::Get().SendEvent(ctfevent);
}

::ctf::ProgramState::ClientDatas_t CtfClientListSystem::createClientDatas()
{
    ::ctf::ProgramState::ClientDatas_t datas;
    for ( PlayerClientDataMap::iterator it = mPlayerToClientData.begin(); it != mPlayerToClientData.end(); ++it )
    {
        datas.push_back( it->second );
    }
    return datas;
}

}
