#ifndef INCLUDED_NETWORK_CTF_CLIENT_LIST_SYSTEM_H
#define INCLUDED_NETWORK_CTF_CLIENT_LIST_SYSTEM_H

#include "engine/system.h"
#include "platform/model_value.h"
#include "platform/register.h"
#include "client_list_changed_event.h"
#include "ctf_client_datas_changed_event.h"
#include <vector>

namespace network {
using platform::ModelValue;
using platform::AutoReg;

class CtfClientListSystem : public engine::System
{
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
    typedef std::map<std::string,::ctf::ClientData> PlayerClientDataMap;
    PlayerClientDataMap mPlayerToClientData;
    AutoReg mOnClientListChanged;
    void OnClientListChanged( ClientListChangedEvent const& event );
    ::ctf::ProgramState::ClientDatas_t createClientDatas();
public:
    DEFINE_SYSTEM_BASE(CtfClientListSystem)
    CtfClientListSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
    std::vector<std::string> blueNames();
    std::vector<std::string> redNames();
    void switchTeam( std::string const & player );
    void removeall( core::ProgramState::ClientDatas_t & from , PlayerClientDataMap const & what );
};

}

#endif
