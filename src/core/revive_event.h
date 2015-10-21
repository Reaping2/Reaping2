#ifndef INCLUDED_CORE_REVIVE_EVENT_H
#define INCLUDED_CORE_REVIVE_EVENT_H

#include "platform/event.h"

namespace core {

struct ReviveEvent : public platform::Event
{
    Opt<ClientData> mClientData;
    ReviveEvent(Opt<ClientData> clientData)
        :mClientData(clientData){}
};

} // namespace core

#endif//INCLUDED_CORE_REVIVE_EVENT_H

//command:  "classgenerator.exe" -c "revive_event" -g "event" -n "core" -m "Opt<ClientData>-clientData"
