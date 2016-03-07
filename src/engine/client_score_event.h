#ifndef INCLUDED_ENGINE_CLIENT_SCORE_EVENT_H
#define INCLUDED_ENGINE_CLIENT_SCORE_EVENT_H

#include "platform/event.h"

namespace engine {

struct ClientScoreEvent : public platform::Event
{
    int32_t mClientID;
    int32_t mScore;
    ClientScoreEvent( int32_t clientID, int32_t score )
        : mClientID( clientID ), mScore( score ) {}
};

} // namespace engine

#endif//INCLUDED_ENGINE_CLIENT_SCORE_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "client_score_event" -m "int32_t-clientID int32_t-score"
