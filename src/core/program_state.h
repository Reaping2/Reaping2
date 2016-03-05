#ifndef INCLUDED_CORE_PROGRAM_STATE_H
#define INCLUDED_CORE_PROGRAM_STATE_H

#include "platform/singleton.h"
#include "platform/i_platform.h"
#include "actor.h"
#include "soldier_properties.h"
#include "platform/export.h"

namespace core {
struct ClientData
{
    int32_t mClientId;
    std::string mClientName;
    int32_t mClientActorGUID;
    SoldierProperties mSoldierProperties;
    int32_t mKill;
    int32_t mDeath;
    int32_t mAssist; // not used yet
    int32_t mScore;
    bool mReady;
    bool mConnected;
    ClientData();

    ClientData(int32_t clientId, std::string const& clientName);

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
    ar & mKill;
    ar & mDeath;
    ar & mAssist;
    ar & mScore;
    ar & mReady;
    ar & mConnected;
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
    enum GameState
    {
        NotRunning,
        Running
    };
    // which mode is the game at currently. (running etc.)
    GameState mGameState;
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
    // a string representation of game mode. ("ctf" or "ffa" or any later extension)
    std::string mGameMode;
    typedef std::vector<ClientData> ClientDatas_t;
    // currently connected clients to server
    ClientDatas_t mClientDatas;
    Opt<ClientData> FindClientDataByClientId(int32_t clientId); 
    Opt<ClientData> FindClientDataByActorGUID(int32_t actorGUID); 
    Opt<ClientData> FindClientDataByClientName(std::string clientName); 
};
} // namespace core

REAPING2_CLASS_EXPORT_KEY2(__core__ClientData, ::core::ClientData,"client_data");
#endif//INCLUDED_CORE_PROGRAM_STATE_H
