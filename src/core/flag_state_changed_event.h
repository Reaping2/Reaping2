#ifndef INCLUDED_CTF_FLAG_STATE_CHANGED_EVENT_H
#define INCLUDED_CTF_FLAG_STATE_CHANGED_EVENT_H

#include "platform/event.h"
#include "ctf_program_state.h"

namespace ctf {

struct FlagStateChangedEvent : public platform::Event
{
    enum Type : int32_t
    {
        Captured = 0,
        Returned,
        Scored,
        Dropped
    };
    Type mType;
    Team::Type mTeam;
    int32_t mCarrierGUID;
    int32_t mFlagGUID;
    FlagStateChangedEvent( Type type, Team::Type team, int32_t carrierGUID, int32_t flagGUID )
        : mType( type ), mTeam( team ), mCarrierGUID( carrierGUID ), mFlagGUID( flagGUID ) {}
};

} // namespace ctf

#endif//INCLUDED_CTF_FLAG_STATE_CHANGED_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "flag_state_changed_event" -m "State-state Team::Type-team" -n "ctf"
