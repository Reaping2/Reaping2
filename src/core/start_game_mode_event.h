#ifndef INCLUDED_CORE_START_GAME_MODE_EVENT_H
#define INCLUDED_CORE_START_GAME_MODE_EVENT_H

#include "platform/event.h"
#include "core/game_modes.h"

namespace core {

struct StartGameModeEvent : public platform::Event
{
    GameModes::Type mMode;
    StartGameModeEvent( GameModes::Type mode )
        : mMode( mode ) {}
};

} // namespace core

#endif//INCLUDED_CORE_START_GAME_MODE_EVENT_H

//command:  "classgenerator.exe" -c "start_game_mode_event" -g "event" -n "core" -m "std::string-mode"
