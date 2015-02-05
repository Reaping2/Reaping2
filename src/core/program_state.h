#ifndef INCLUDED_CORE_PROGRAM_STATE_H
#define INCLUDED_CORE_PROGRAM_STATE_H

#include "platform/singleton.h"
#include "platform/i_platform.h"

namespace core {
class ProgramState : public platform::Singleton<ProgramState>
{
    friend class platform::Singleton<ProgramState>;

public:
    ProgramState();
    enum Mode
    {
        Local,
        Client,
        Server
    };
    Mode mMode;
    bool mClientConnected;
    std::string mServerIp;
    std::string mClientName;
    std::string mClientId;
};
} // namespace core
#endif//INCLUDED_CORE_PROGRAM_STATE_H