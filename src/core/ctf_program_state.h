#ifndef INCLUDED_CTF_PROGRAM_STATE_H
#define INCLUDED_CTF_PROGRAM_STATE_H

#include "platform/singleton.h"
#include "platform/i_platform.h"
#include "opt.h"

namespace ctf {
struct ClientData
{
    int32_t mClientId;
    enum Team
    {
        Blue=0,
        Red
    };
    Team mTeam;
    ClientData()
        :mClientId(0)
        ,mTeam(Blue)
    {
    }

    ClientData(int32_t clientId, Team team)
        :mClientId(clientId),mTeam(team) {}

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void ClientData::serialize(Archive& ar, const unsigned int version)
{
    ar & mClientId;
    ar & mTeam;
}
class ProgramState : public platform::Singleton<ctf::ProgramState>
{
    friend class platform::Singleton<ctf::ProgramState>;

public:
    ProgramState();
    typedef std::vector<ClientData> ClientDatas_t;
    // currently connected clients to server
    ClientDatas_t mClientDatas;
    Opt<ClientData> FindClientDataByClientId(int32_t clientId); 
};
} // namespace ctf
#endif//INCLUDED_CTF_PROGRAM_STATE_H