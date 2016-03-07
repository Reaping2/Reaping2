#ifndef INCLUDED_NETWORK_CLIENT_ID_CHANGED_H
#define INCLUDED_NETWORK_CLIENT_ID_CHANGED_H

#include "platform/event.h"

namespace network {

struct ClientIdChangedEvent : public platform::Event
{
    int32_t mCurrClientId;
    int32_t mNewClientId;
    ClientIdChangedEvent( int32_t currClientId, int32_t newClientId )
        : mCurrClientId( currClientId ), mNewClientId( newClientId ) {}
};

} // namespace network

#endif//INCLUDED_NETWORK_CLIENT_ID_CHANGED_H

//command:  "classgenerator.exe" -g "event" -c "client_id_changed" -m "int32_t-currClientId int32_t-newClientId" -n "network"
