#ifndef INCLUDED_CTF_PROGRAM_STATE_H
#define INCLUDED_CTF_PROGRAM_STATE_H

#include "platform/singleton.h"
#include "platform/i_platform.h"
#include "opt.h"
#include <map>
#include <vector>

class Team : public platform::Singleton<Team>
{
protected:
    friend class platform::Singleton<Team>;
    Team();
public:
    enum Type
    {
        Red=0,
        Blue
    };
    Team::Type operator()( int32_t Id ) const;
private:
    typedef std::map<int32_t,Team::Type> IdToTeamMap_t;
    IdToTeamMap_t mIdToTeamMap;
};


namespace ctf {

struct ClientData
{
    int32_t mClientId;
    Team::Type mTeam;
    std::string mClientName;
    ClientData()
        :mClientId(0)
        ,mTeam(Team::Blue)
        ,mClientName("")
    {
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void ClientData::serialize(Archive& ar, const unsigned int version)
{
    ar & mClientId;
    ar & mTeam;
    ar & mClientName;
}
class ProgramState : public platform::Singleton<ctf::ProgramState>
{
    friend class platform::Singleton<ctf::ProgramState>;
public:
    ProgramState();

    void ResetScores();

    int32_t mBlueScore;
    int32_t mRedScore;
    int32_t const& GetBlueScore();
    int32_t const& GetRedScore();
    typedef std::vector<ClientData> ClientDatas_t;
    // currently connected clients to server
    ClientDatas_t mClientDatas;
    Opt<ClientData> FindClientDataByClientId(int32_t clientId); 
};
} // namespace ctf
#endif//INCLUDED_CTF_PROGRAM_STATE_H
