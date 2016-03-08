#ifndef INCLUDED_ENGINE_SHOW_TEXT_EVENT_H
#define INCLUDED_ENGINE_SHOW_TEXT_EVENT_H

#include "platform/event.h"

namespace engine {

struct ShowTextEvent : public platform::Event
{
    double mSeconds;
    std::string mText;
    ShowTextEvent( double seconds, std::string text )
        : mSeconds( seconds ), mText( text ) {}
};

} // namespace engine

#endif//INCLUDED_ENGINE_SHOW_TEXT_EVENT_H

//command:  "classgenerator.exe" -g "event" -c "show_text_event" -m "double-seconds std::string-text"
