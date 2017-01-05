#ifndef INCLUDED_NETWORK_CTF_CLIENT_LIST_SYSTEM_H
#define INCLUDED_NETWORK_CTF_CLIENT_LIST_SYSTEM_H

#include "engine/system.h"
#include "platform/model_value.h"
#include "platform/register.h"
#include "core/ctf_program_state.h"
#include <vector>

namespace network {
using platform::ModelValue;
using platform::AutoReg;
struct CtfClientDatasChangedEvent;

class CtfClientListDisplayingSystem : public engine::System
{
    ModelValue mCTFModel;
    ModelValue mSwitchModel;
    ModelValue mBlueNamesModel;
    ModelValue mRedNamesModel;
    // strings are for the UI
    std::vector<std::string> mRedNames;
    std::vector<std::string> mBlueNames;
    ::ctf::ProgramState::ClientDatas_t mCtfClientDatas;
    AutoReg mOnCtfClientDatasChanged;
    void OnCtfClientDatasChanged( CtfClientDatasChangedEvent const& event );
public:
    DEFINE_SYSTEM_BASE( CtfClientListDisplayingSystem )
    CtfClientListDisplayingSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
    std::vector<std::string> BlueNames();
    std::vector<std::string> RedNames();
    void SwitchTeam( std::string const& player );
};

}

#endif
