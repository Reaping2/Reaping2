#include "core/program_state.h"
namespace core {

    ProgramState::ProgramState()
        : mMode(ProgramState::Local)
        , mClientConnected(false)
        , mControlledActorGUID(-1)
    {
    }

} // namespace core