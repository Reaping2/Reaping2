#ifndef INCLUDED_MAP_EDITOR_BACK_EVENT_H
#define INCLUDED_MAP_EDITOR_BACK_EVENT_H

#include "platform/event.h"

namespace map {

struct EditorBackEvent : public platform::Event
{
    bool mBackToBaseHud;
    EditorBackEvent( bool backToBaseHud = false )
        : mBackToBaseHud(backToBaseHud){}
};

} // namespace map

#endif//INCLUDED_MAP_EDITOR_BACK_EVENT_H

//command:  "classgenerator.exe" -g "event" -n "map" -c "editor_back_event"
