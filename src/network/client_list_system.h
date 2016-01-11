#ifndef INCLUDED_NETWORK_CLIENT_LIST_SYSTEM_H
#define INCLUDED_NETWORK_CLIENT_LIST_SYSTEM_H

#include "engine/system.h"
#include "platform/model_value.h"
#include "platform/register.h"
#include "client_list_changed_event.h"
#include <vector>

namespace network {
using platform::ModelValue;

class ClientListSystem : public engine::System
{
	ModelValue mGameModel;
	ModelValue mCTFModel;
	ModelValue mClientsModel;
	std::vector<std::string> mClients;
	platform::AutoReg mOnClientListChanged;
	void OnClientListChanged( ClientListChangedEvent const& event );
	public:
	DEFINE_SYSTEM_BASE(ClientListSystem)
	ClientListSystem();
	virtual void Init();
	virtual void Update( double DeltaTime );
	std::vector<std::string> getClients();
};

}

#endif
