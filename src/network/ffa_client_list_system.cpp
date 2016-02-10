#include "ffa_client_list_system.h"

namespace network {
using platform::RootModel;

FfaClientListSystem::FfaClientListSystem()
    : mFFAModel("ffa_client_list",&RootModel::Get())
    , mClientListModel( (ModelValue::get_string_vec_t)boost::bind( &FfaClientListSystem::clients, this),"clients", &mFFAModel )
{
    mOnClientListChanged = platform::EventServer<network::ClientListChangedEvent>::Get().Subscribe( boost::bind( &FfaClientListSystem::OnClientListChanged, this, _1) );
}

void FfaClientListSystem::Init()
{
}

void FfaClientListSystem::Update( double DeltaTime )
{
}

std::vector<std::string> FfaClientListSystem::clients()
{
    return mClientList;
}

void FfaClientListSystem::OnClientListChanged( ClientListChangedEvent const & event )
{
    core::ProgramState::ClientDatas_t const & clients = event.mClientList;
    std::vector<std::string> clientList;
    std::for_each( clients.begin(), clients.end(),
            boost::bind<void>([]( core::ClientData const & d, std::vector<std::string> & c ){ c.push_back(d.mClientName); }, _1, boost::ref(clientList)));
    std::swap(clientList, mClientList);
}

}
