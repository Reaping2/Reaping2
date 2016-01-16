#ifndef INCLUDED_NETWORK_CLIENT_LIST_SYSTEM_H
#define INCLUDED_NETWORK_CLIENT_LIST_SYSTEM_H

#include "engine/system.h"
#include "platform/model_value.h"
#include "platform/register.h"
#include "client_list_changed_event.h"
#include "ctf_client_datas_changed_event.h"
#include <vector>

namespace network {
using platform::ModelValue;

class ClientListSystem : public engine::System
{
    ModelValue mGameModel;
    ModelValue mCTFModel;
    ModelValue mSwitchModel;
    ModelValue mRedTeamModel;
    ModelValue mBlueTeamModel;
    ModelValue mBlueNamesModel;
    ModelValue mRedNamesModel;
    // strings are for the UI
    std::vector<std::string> mRedNames;
    std::vector<std::string> mBlueNames;
    // ids are for the lifecycle subsystem
    std::vector<int32_t> mRedIds;
    std::vector<int32_t> mBlueIds;
    platform::AutoReg mOnClientListChanged;
    void OnClientListChanged( ClientListChangedEvent const& event );
    ::ctf::ProgramState::ClientDatas_t createClientDatas();
public:
    DEFINE_SYSTEM_BASE(ClientListSystem)
    ClientListSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
    std::vector<std::string> blueNames();
    std::vector<std::string> redNames();
    void switchTeam( std::string const & player );
};

}

#endif
