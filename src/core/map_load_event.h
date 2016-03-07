#ifndef INCLUDED_CORE_MAP_LOAD_EVENT_H
#define INCLUDED_CORE_MAP_LOAD_EVENT_H

#include "platform/event.h"

namespace core {

struct MapLoadEvent : public platform::Event
{
    std::string mMapName;
    MapLoadEvent( std::string const& mapName )
        : mMapName( mapName ) {}
};

} // namespace core

#endif//INCLUDED_CORE_MAP_LOAD_EVENT_H
