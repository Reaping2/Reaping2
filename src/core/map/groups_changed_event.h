#ifndef INCLUDED_MAP_GROUPS_CHANGED_EVENT_H
#define INCLUDED_MAP_GROUPS_CHANGED_EVENT_H

#include "platform/event.h"

namespace map {

struct GroupsChangedEvent : public platform::Event
{
    GroupsChangedEvent(){}
};

} // namespace map

#endif//INCLUDED_MAP_GROUPS_CHANGED_EVENT_H

//command:  "classgenerator.exe" -g "event" -n "map" -c "groups_changed_event"
