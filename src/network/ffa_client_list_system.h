#ifndef INCLUDED_NETWORK_FFA_CLIENT_LIST_SYSTEM_H
#define INCLUDED_NETWORK_FFA_CLIENT_LIST_SYSTEM_H

#include "engine/system.h"
#include "platform/model_value.h"
#include "platform/register.h"
#include "client_list_changed_event.h"

namespace network {
using platform::ModelValue;
using platform::AutoReg;

class FfaClientListSystem : public engine::System
{
    ModelValue mFFAModel;
    ModelValue mClientListModel;

    std::vector<std::string> mClientList;

    AutoReg mOnClientListChanged;
    void OnClientListChanged( ClientListChangedEvent const & event );
public:
    DEFINE_SYSTEM_BASE(FfaClientListSystem)
    FfaClientListSystem();
    virtual void Init();
    virtual void Update(double DeltaTime);
    std::vector<std::string> clients();
};

}

#endif
