#ifndef INCLUDED_MAP_EDITOR_MODE_CHANGED_EVENT_H
#define INCLUDED_MAP_EDITOR_MODE_CHANGED_EVENT_H

#include "platform/event.h"

namespace map {

struct EditorModeChangedEvent : public platform::Event
{
    std::string mMode;
    std::string mPrevMode;
    EditorModeChangedEvent(std::string mode, std::string prevMode )
        : mMode(mode)
        , mPrevMode(prevMode){}
};

} // namespace map

#endif//INCLUDED_MAP_EDITOR_MODE_CHANGED_EVENT_H

//command:  "classgenerator.exe" -g "event" -n "map" -c "editor_mode_changed_event" -m "std::string-mode"
