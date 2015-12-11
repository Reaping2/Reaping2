#ifndef INCLUDED_ENGINE_KILL_SCORE_EVENT_H
#define INCLUDED_ENGINE_KILL_SCORE_EVENT_H

#include "platform/event.h"

namespace engine {

struct KillScoreEvent : public platform::Event
{
    int32_t mKillerGUID;
    int32_t mDeadGUID;
    KillScoreEvent(int32_t killerGUID, int32_t deadGUID)
        :mKillerGUID(killerGUID),mDeadGUID(deadGUID){}
};

} // namespace engine

#endif//INCLUDED_ENGINE_KILL_SCORE_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "kill_score_event" -m "int32_t-killerGUID int32_t-deadGUID" -n "engine"
