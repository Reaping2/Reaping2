#ifndef INCLUDED_ENGINE_CLIENT_READY_EVENT_H
#define INCLUDED_ENGINE_CLIENT_READY_EVENT_H

#include "platform/event.h"

namespace network {

struct ClientReadyEvent : public platform::Event
{
    int32_t mClientId;
    std::string mClientName;
    ClientReadyEvent(){}
};

} // namespace engine

#endif//INCLUDED_ENGINE_CLIENT_READY_EVENT_H

//command:  "./classgenerator" -g "event" -c "client_ready_event"
