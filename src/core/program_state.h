#ifndef INCLUDED_CORE_PROGRAM_STATE_H
#define INCLUDED_CORE_PROGRAM_STATE_H

#include "platform/singleton.h"
#include "platform/i_platform.h"
#include "actor.h"

namespace core {
struct ClientData
{
    int32_t mClientId;
    std::string mClientName;
    Opt<Actor> mClientActor;
    ClientData(int32_t clientId, std::string const& clientName):mClientId(clientId),mClientName(clientName) {}
};
class ProgramState : public platform::Singleton<ProgramState>
{
    //TODO: should be split into serverstate clientstate
    friend class platform::Singleton<ProgramState>;

public:
    platform::ModelValue mProgramStateModel;
    platform::ModelValue mIsClientModel;
    int32_t mIsClient;
    ProgramState();
    enum Mode
    {
        Local,
        Client,
        Server
    };
    // main type of this instance (local,client,server)
    Mode mMode;
    void SetMode(ProgramState::Mode mode);
    // is this client connected to server
    bool mClientConnected;
    // current client's name
    std::string mClientName;
    // current client's id (got from server in exchange for name)
    int32_t mClientId;
    // currently controlled actor for client //TODO: need to find a better place
    int32_t mControlledActorGUID;

    // target servers ip
    std::string mServerIp;
    typedef std::vector<ClientData> ClientDatas_t;
    // currently connected clients to server
    ClientDatas_t mClientDatas;
    
};
} // namespace core
#endif//INCLUDED_CORE_PROGRAM_STATE_H