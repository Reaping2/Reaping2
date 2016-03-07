#ifndef INCLUDED_NETWORK_LOAD_CLIENTLIST_EVENT_H
#define INCLUDED_NETWORK_LOAD_CLIENTLIST_EVENT_H

#include "platform/event.h"

namespace network {

struct LoadClientlistEvent : public platform::Event
{
    std::string mGameMode;
    LoadClientlistEvent() {}
};

} // namespace engine

#endif//INCLUDED_NETWORK_LOAD_CLIENTLIST_EVENT_H
