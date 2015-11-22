#include "core/program_state.h"
#include "platform/model_value.h"
namespace core {

    ProgramState::ProgramState()
        : mMode(ProgramState::Local)
        , mClientConnected(false)
        , mControlledActorGUID(-1)
        , mProgramStateModel( "programstate", &platform::RootModel::Get() )
        , mIsClientModel( mIsClient, "isclient", &mProgramStateModel )
        , mIsClient(0)
    {
    }

    void ProgramState::SetMode(ProgramState::Mode mode)
    {
        mMode=mode;
        mIsClient=mMode==ProgramState::Client;
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

    Opt<ClientData> ProgramState::FindClientDataByActorGUID(int32_t actorGUID)
    {
        for (ClientDatas_t::iterator i=mClientDatas.begin(), e=mClientDatas.end();i!=e;++i)
        {
            if (i->mClientActorGUID==actorGUID)
            {
                return Opt<ClientData>(&*i);
            }
        }
        return Opt<ClientData>();
    }


    ClientData::ClientData() 
        :mClientId(0)
        ,mClientActorGUID(-1)
    {

    }

    ClientData::ClientData(int32_t clientId, std::string const& clientName)
        :mClientId(clientId)
        ,mClientName(clientName)
        ,mClientActorGUID(-1)

    {

    }

} // namespace core