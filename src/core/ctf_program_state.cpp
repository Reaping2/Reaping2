#include "core/ctf_program_state.h"


using platform::AutoId;

Team::Team()
{
    mIdToTeamMap[AutoId("blue")]=Blue;
    mIdToTeamMap[AutoId("red")]=Red;
}

Team::Type Team::operator()( int32_t Id ) const
{
    IdToTeamMap_t::const_iterator i=mIdToTeamMap.find(Id);
    BOOST_ASSERT(i!=mIdToTeamMap.end());
    return (i!=mIdToTeamMap.end())?i->second:Blue;
}

namespace ctf {

ProgramState::ProgramState()
    : mBlueScore(0)
    , mRedScore(0)
{
    ResetScores();
}

Opt<ClientData> ProgramState::FindClientDataByClientId(int32_t clientId)
{
    for (ClientDatas_t::iterator i=mClientDatas.begin(), e=mClientDatas.end();i!=e;++i)
    {
        if (i->mClientId==clientId)
        {
            return Opt<ClientData>(&*i);
        }
    }
    return Opt<ClientData>();
}

void ProgramState::ResetScores()
{
    mBlueScore=0;
    mRedScore=0;
}

int32_t const& ProgramState::GetBlueScore()
{
    return mBlueScore;
}

int32_t const& ProgramState::GetRedScore()
{
    return mRedScore;
}


} // namespace ctf