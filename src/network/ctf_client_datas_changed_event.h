#ifndef INCLUDED_NETWORK_CTF_CLIENT_DATAS_CHANGED_EVENT_H
#define INCLUDED_NETWORK_CTF_CLIENT_DATAS_CHANGED_EVENT_H

#include "platform/event.h"
#include "core/ctf_program_state.h"

namespace network {

struct CtfClientDatasChangedEvent : public platform::Event
{
    ::ctf::ProgramState::ClientDatas_t mCtfClientDatas;
    CtfClientDatasChangedEvent() {}
};

} // namespace engine

#endif//INCLUDED_NETWORK_CTF_CLIENT_DATAS_CHANGED_EVENT_H
