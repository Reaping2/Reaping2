#include "core/program_state.h"
namespace core {

    ProgramState::ProgramState()
        : mMode(ProgramState::Local)
        , mClientConnected(false)
    {
    }

} // namespace core