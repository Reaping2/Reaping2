#ifndef INCLUDED_ENGINE_TEAM_SWITCH_REQUEST_EVENT_H
#define INCLUDED_ENGINE_TEAM_SWITCH_REQUEST_EVENT_H

#include "platform/event.h"

namespace network {

struct TeamSwitchRequestEvent : public platform::Event
{
    int32_t mClientId;
    TeamSwitchRequestEvent(){}
};

} // namespace engine

#endif//INCLUDED_ENGINE_TEAM_SWITCH_REQUEST_EVENT_H

//command:  "./classgenerator" -g "event" -c "team_switch_request_event"
