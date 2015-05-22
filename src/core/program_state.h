#ifndef INCLUDED_CORE_PROGRAM_STATE_H
#define INCLUDED_CORE_PROGRAM_STATE_H

#include "platform/singleton.h"
#include "platform/i_platform.h"
#include "actor.h"
#include "soldier_properties.h"

namespace core {
struct ClientData
{
    int32_t mClientId;
    std::string mClientName;
    int32_t mClientActorGUID;
    SoldierProperties mSoldierProperties;
    ClientData()
        :mClientId(0)
        ,mClientActorGUID(0)
    {
    }

    ClientData(int32_t clientId, std::string const& clientName)
        :mClientId(clientId),mClientName(clientName) {}

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void ClientData::serialize(Archive& ar, const unsigned int version)
{
    ar & mClientId;
    ar & mClientName;
    ar & mClientActorGUID;
    ar & mSoldierProperties;
}
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
    SoldierProperties mSoldierProperties;

    // target servers ip
    std::string mServerIp;
    typedef std::vector<ClientData> ClientDatas_t;
    // currently connected clients to server
    ClientDatas_t mClientDatas;
    Opt<ClientData> FindClientDataByClientId(int32_t clientId); 
    Opt<ClientData> FindClientDataByActorGUID(int32_t actorGUID); 
};
} // namespace core
#endif//INCLUDED_CORE_PROGRAM_STATE_H