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
{
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


} // namespace ctf