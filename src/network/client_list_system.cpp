#include "client_list_system.h"
#include "platform/event.h"
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
    , mBlueIdsModel( (ModelValue::get_int_vec_t) boost::bind( &ClientListSystem::blueIds, this) , "ids", &mRedTeamModel)
    , mRedIdsModel( (ModelValue::get_int_vec_t) boost::bind( &ClientListSystem::redIds, this) , "ids", &mRedTeamModel)
{
	mOnClientListChanged = platform::EventServer<network::ClientListChangedEvent>::Get().Subscribe( boost::bind( &ClientListSystem::OnClientListChanged, this, _1 ) );
    // TODO: bind here the team change button
    // and on team change send the ctf programstate to the server
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

std::vector<int32_t> ClientListSystem::blueIds()
{
    return mBlueIds;
}

std::vector<int32_t> ClientListSystem::redIds()
{
    return mRedIds;
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
}

}
