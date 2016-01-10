#include "client_list_system.h"
#include "platform/event.h"
#include <boost/bind.hpp>
#include <utility>

namespace network {

using platform::RootModel;

ClientListSystem::ClientListSystem()
	: mGameModel("game", &RootModel::Get())
	, mCTFModel("ctf", &mGameModel)
    , mClientsModel( "clients", &mCTFModel )
    , mBlueTeamModel( (ModelValue::get_string_vec_t) boost::bind( &ClientListSystem::blueTeam, this) , "blue", &mClientsModel )
    , mReadTeamModel( (ModelValue::get_string_vec_t) boost::bind( &ClientListSystem::redTeam, this) , "red", &mClientsModel )
{
	mOnClientListChanged = platform::EventServer<network::ClientListChangedEvent>::Get().Subscribe( boost::bind( &ClientListSystem::OnClientListChanged, this, _1 ) );
}

void ClientListSystem::Init()
{
}

void ClientListSystem::Update( double DeltaTime )
{
}

std::vector<std::string> ClientListSystem::blueTeam()
{
	return mBlueTeam;
}

std::vector<std::string> ClientListSystem::redTeam()
{
    return mRedTeam;
}

namespace {
    void removeall( std::vector<std::string> & from, std::vector<std::string> const & what )
    {
        for ( std::vector<std::string>::const_iterator whatit = what.begin(); whatit != what.end(); ++whatit )
        {
            std::vector<std::string>::iterator it = std::find( from.begin(), from.end(), *whatit );
            if ( from.end() != it )
            {
                from.erase( it );
            }
        }
        std::for_each( what.begin(), what.end(), 
                boost::bind<void>( [](std::string const & s, std::vector<std::string> & v) { std::remove( v.begin(), v.end(),s); }, _1, boost::ref(from) ) ); 
    }
}

void ClientListSystem::OnClientListChanged( ClientListChangedEvent const& event )
{
    std::vector<std::string> clients = event.mClientList;
    // TODO; what about quit clients
    // TODO: multiple clients with the same name
    // remove all blues and reds from clients -> new clients
    removeall( clients, mBlueTeam );
    removeall( clients, mRedTeam );
    // simple distribute: add to the smaller team
    for ( size_t i = 0; i < clients.size(); ++i )
    {
        if ( mBlueTeam.size() < mRedTeam.size() )
        {
            mBlueTeam.push_back(clients[i]);
        }
        else
        {
            mRedTeam.push_back(clients[i]);
        }
    }
}

}
