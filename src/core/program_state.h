#ifndef INCLUDED_CORE_PROGRAM_STATE_H
#define INCLUDED_CORE_PROGRAM_STATE_H

#include "platform/singleton.h"
#include "platform/i_platform.h"

namespace core {
struct ClientData
{
    int32_t mClientId;
    std::string mClientName;
    ClientData(int32_t clientId, std::string const& clientName):mClientId(clientId),mClientName(clientName) {}
};
class ProgramState : public platform::Singleton<ProgramState>
{
    //TODO: should be split into serverstate clientstate
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
    int32_t mClientId;
    typedef std::vector<ClientData> ClientDatas_t;
    ClientDatas_t mClientDatas;
    
};
} // namespace core
#endif//INCLUDED_CORE_PROGRAM_STATE_H