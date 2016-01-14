#include "client_list_system.h"
#include "platform/event.h"
#include "ctf_client_datas_message.h"
#include <algorithm>

namespace network {

using platform::RootModel;

ClientListSystem::ClientListSystem()
	: mGameModel("game", &RootModel::Get())
	, mCTFModel("ctf", &mGameModel)
    , mRedTeamModel( "red", &mCTFModel )
    , mBlueTeamModel( "blue", &mCTFModel )
    , mBlueNamesModel( (ModelValue::get_string_vec_t) boost::bind( &ClientListSystem::blueNames, this) , "names", &mBlueTeamModel)
    , mRedNamesModel( (ModelValue::get_string_vec_t) boost::bind( &ClientListSystem::redNames, this) , "names", &mRedTeamModel)
    , mSwitchModel( StringFunc( this, &ClientListSystem::switchTeam ), "switch", &mCTFModel )
{
	mOnClientListChanged = platform::EventServer<network::ClientListChangedEvent>::Get().Subscribe( boost::bind( &ClientListSystem::OnClientListChanged, this, _1 ) );
}

void ClientListSystem::Init()
{
}

void ClientListSystem::Update( double DeltaTime )
{
}

std::vector<std::string> ClientListSystem::blueNames()
{
	return mBlueNames;
}

std::vector<std::string> ClientListSystem::redNames()
{
    return mRedNames;
}

namespace {
    void removeall( core::ProgramState::ClientDatas_t & from , std::vector<int32_t> const & what )
    {
        for ( size_t i = 0; i < what.size(); ++i )
        {
            for ( core::ProgramState::ClientDatas_t::iterator it = from.begin(); it != from.end(); ++it )
            {
                if ( (*it).mClientId == what[i] )
                {
                    from.erase( it );
                    break;
                }
            }
        }
    }
}

void ClientListSystem::OnClientListChanged( ClientListChangedEvent const& event )
{
    core::ProgramState::ClientDatas_t const & clients = event.mClientList;
    core::ProgramState::ClientDatas_t readyClients(clients.size());
    auto it = std::copy_if( clients.begin(), clients.end(), readyClients.begin(), [] ( core::ClientData const & d ) { return d.mReady; } );
    readyClients.resize(std::distance(readyClients.begin(), it));
    // TODO; what about quit clients
    // TODO: multiple clients with the same name
    // remove all blues and reds from clients -> new clients
    removeall( readyClients, mBlueIds );
    removeall( readyClients, mRedIds );
    // simple distribute: add to the smaller team
    for ( size_t i = 0; i < readyClients.size(); ++i )
    {
        if ( mBlueNames.size() < mRedNames.size() )
        {
            mBlueNames.push_back(readyClients[i].mClientName);
            mBlueIds.push_back(readyClients[i].mClientId);
        }
        else
        {
            mRedNames.push_back(readyClients[i].mClientName);
            mRedIds.push_back(readyClients[i].mClientId);
        }
    }
    // TODO: send here
    // send ctfclientdatachanged event
    // subscribe to that in clientdatasmessage
}

void ClientListSystem::switchTeam( std::string const & player )
{
    std::vector<std::string>::iterator nameit = std::find(mBlueNames.begin(), mBlueNames.end(), player );
    int distance = std::distance( mBlueNames.begin(), nameit );
    if ( mBlueNames.size() == distance )
    {
        nameit = std::find(mRedNames.begin(), mRedNames.end(), player );
        distance = std::distance( mRedNames.begin(), nameit );

        mRedNames.erase(nameit);
        std::vector<int32_t>::iterator idit = mRedIds.begin()+distance;
        int32_t id = *idit;
        mRedIds.erase(mRedIds.begin()+distance);
        mBlueNames.push_back(player);
        mBlueIds.push_back(id);
    }
    else
    {
        mBlueNames.erase(nameit);
        std::vector<int32_t>::iterator idit = mBlueIds.begin()+distance;
        int32_t id = *idit;
        mBlueIds.erase(mBlueIds.begin()+distance);
        mRedNames.push_back(player);
        mRedIds.push_back(id);
    }
    // TODO: send here
}

}
