#ifndef INCLUDED_CORE_MAP_LOADED_EVENT_H
#define INCLUDED_CORE_MAP_LOADED_EVENT_H

#include "platform/event.h"

namespace core {

struct MapLoadedEvent : public platform::Event
{
    MapLoadedEvent(){}
};

} // namespace core

#endif//INCLUDED_CORE_MAP_LOADED_EVENT_H
