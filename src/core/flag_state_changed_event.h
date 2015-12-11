#ifndef INCLUDED_CTF_FLAG_STATE_CHANGED_EVENT_H
#define INCLUDED_CTF_FLAG_STATE_CHANGED_EVENT_H

#include "platform/event.h"

namespace ctf {

struct FlagStateChangedEvent : public platform::Event
{
    enum Type
    {
        Captured=0,
        Returned,
        Scored
    };
    Type mType;
    Team::Type mTeam;
    int32_t mCarrierGUID;
    FlagStateChangedEvent(Type type, Team::Type team, int32_t carrierGUID)
        :mType(type),mTeam(team),mCarrierGUID(carrierGUID){}
};

} // namespace ctf

#endif//INCLUDED_CTF_FLAG_STATE_CHANGED_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "flag_state_changed_event" -m "State-state Team::Type-team" -n "ctf"
