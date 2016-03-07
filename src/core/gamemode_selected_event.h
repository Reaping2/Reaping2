#ifndef INCLUDED_CORE_GAMEMODE_SELECTED_EVENT_H
#define INCLUDED_CORE_GAMEMODE_SELECTED_EVENT_H

#include "platform/event.h"

namespace core {

struct GamemodeSelectedEvent : public platform::Event
{
    std::string mGameMode;
    GamemodeSelectedEvent() {}
};

} // namespace core

#endif//INCLUDED_CORE_GAMEMODE_SELECTED_EVENT_H

//command:  "./classgenerator" -g "event" -c "gamemode_selected_event"
