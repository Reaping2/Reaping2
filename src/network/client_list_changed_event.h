#ifndef INCLUDED_NETWORK_CLIENT_LIST_CHANGED_EVENT_H
#define INCLUDED_NETWORK_CLIENT_LIST_CHANGED_EVENT_H

#include "platform/event.h"
#include "core/program_state.h"

namespace network {

struct ClientListChangedEvent : public platform::Event
{
    core::ProgramState::ClientDatas_t mClientList;
    ClientListChangedEvent( core::ProgramState::ClientDatas_t & clients )
        : mClientList(clients)
    {
    }
};

}

#endif
