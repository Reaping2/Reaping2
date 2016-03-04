#ifndef INCLUDED_CORE_LEVEL_SELECTED_EVENT_H
#define INCLUDED_CORE_LEVEL_SELECTED_EVENT_H

#include "platform/event.h"
#include <string>
namespace core {

struct LevelSelectedEvent : public Event
{
    std::string mLevel;
    LevelSelectedEvent( std::string const& level ) : mLevel(level){}
};

}

#endif
