#ifndef INCLUDED_ENGINE_CLIENT_DATAS_CHANGED_EVENT_H
#define INCLUDED_ENGINE_CLIENT_DATAS_CHANGED_EVENT_H

#include "platform/event.h"

namespace engine {

struct ClientDatasChangedEvent : public platform::Event
{
    ClientDatasChangedEvent() {}
};

} // namespace engine

#endif//INCLUDED_ENGINE_CLIENT_DATAS_CHANGED_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "client_datas_changed_event"
