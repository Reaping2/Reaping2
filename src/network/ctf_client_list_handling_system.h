#ifndef INCLUDED_NETWORK_CTF_CLIENT_LIST_HANDLING_SYSTEM_H
#define INCLUDED_NETWORK_CTF_CLIENT_LIST_HANDLING_SYSTEM_H

#include "engine/system.h"
#include "platform/register.h"

namespace network {

struct TeamSwitchRequestEvent;
struct ClientReadyEvent;

using platform::AutoReg;

class CtfClientListHandlingSystem : public engine::System
{
    AutoReg mOnTeamSwitchRequestEvent;
    AutoReg mClientReadyEvent;
    void OnTeamSwitchRequestEvent( TeamSwitchRequestEvent const& event );
    void OnClientReadyEvent( ClientReadyEvent const& event );
public:
    DEFINE_SYSTEM_BASE( CtfClientListHandlingSystem )
    CtfClientListHandlingSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
};

}

#endif
