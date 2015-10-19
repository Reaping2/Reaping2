#include "core/ctf_program_state.h"
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