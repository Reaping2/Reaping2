#ifndef INCLUDED_ENGINE_CONNECTION_EVENT_H
#define INCLUDED_ENGINE_CONNECTION_EVENT_H

#include "platform/event.h"

namespace engine {

struct ConnectionEvent : public platform::Event
{
    int32_t mClientId;
    bool mConnected;
    ConnectionEvent(int32_t clientId, bool connected)
        :mClientId(clientId),mConnected(connected){}
};

} // namespace engine

#endif//INCLUDED_ENGINE_CONNECTION_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "connection_event" -m "int32_t-clientId bool-connected"
