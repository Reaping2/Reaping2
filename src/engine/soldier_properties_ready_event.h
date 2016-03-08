#ifndef INCLUDED_ENGINE_SOLDIER_PROPERTIES_READY_EVENT_H
#define INCLUDED_ENGINE_SOLDIER_PROPERTIES_READY_EVENT_H

#include "platform/event.h"

namespace engine {

struct SoldierPropertiesReadyEvent : public platform::Event
{
    SoldierPropertiesReadyEvent() {}
};

} // namespace engine

#endif//INCLUDED_ENGINE_SOLDIER_PROPERTIES_READY_EVENT_H
