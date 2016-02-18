#ifndef INCLUDED_ENGINE_CONNECTION_EVENT_H
#define INCLUDED_ENGINE_CONNECTION_EVENT_H

#include "platform/event.h"

namespace engine {

struct ConnectionEvent : public platform::Event
{
    enum State
    {
        Connected,
        Disconnected,
        Reconnected
    };
    int32_t mClientId;
    State mConnected;
    ConnectionEvent(int32_t clientId, State connected)
        :mClientId(clientId),mConnected(connected){}
};

} // namespace engine

#endif//INCLUDED_ENGINE_CONNECTION_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "connection_event" -m "int32_t-clientId bool-connected"
