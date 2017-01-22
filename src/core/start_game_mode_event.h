#ifndef INCLUDED_CORE_START_GAME_MODE_EVENT_H
#define INCLUDED_CORE_START_GAME_MODE_EVENT_H

#include "platform/event.h"
#include "core/gamemodes.h"

namespace core {

struct StartGameModeEvent : public platform::Event
{
    GameModes::GameMode mMode;
    StartGameModeEvent( GameModes::GameMode mode )
        : mMode( mode ) {}
};

} // namespace core

#endif//INCLUDED_CORE_START_GAME_MODE_EVENT_H

//command:  "classgenerator.exe" -c "start_game_mode_event" -g "event" -n "core" -m "std::string-mode"
