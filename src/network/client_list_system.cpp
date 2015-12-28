#include "client_list_system.h"
#include "platform/event.h"
#include <boost/bind.hpp>
#include <utility>

namespace network {

using platform::RootModel;

ClientListSystem::ClientListSystem()
	: mGameModel("game", &RootModel::Get())
	, mCTFModel("ctf", &mGameModel)
    , mClientsModel( (ModelValue::get_string_vec_t) boost::bind( &ClientListSystem::getClients, this) , "clients", &mCTFModel )
{
	mOnClientListChanged = platform::EventServer<network::ClientListChangedEvent>::Get().Subscribe( boost::bind( &ClientListSystem::OnClientListChanged, this, _1 ) );
}

void ClientListSystem::Init()
{
}

void ClientListSystem::Update( double DeltaTime )
{
}

std::vector<std::string> ClientListSystem::getClients()
{
	return mClients;
}

void ClientListSystem::OnClientListChanged( ClientListChangedEvent const& event )
{
	mClients = event.mClientList;
}

}
