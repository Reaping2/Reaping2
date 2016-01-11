#ifndef INCLUDED_NETWORK_CLIENT_LIST_CHANGED_EVENT_H
#define INCLUDED_NETWORK_CLIENT_LIST_CHANGED_EVENT_H

#include "platform/event.h"

namespace network {

struct ClientListChangedEvent : public platform::Event
{
    std::vector<std::string> mClientList;
    ClientListChangedEvent( std::vector<std::string> clients )
        : mClientList(clients)
    {
    }
};

}

#endif
