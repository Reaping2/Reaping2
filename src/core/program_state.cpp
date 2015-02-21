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

} // namespace core