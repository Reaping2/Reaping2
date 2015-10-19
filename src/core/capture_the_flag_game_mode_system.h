#ifndef INCLUDED_CORE_CAPTURE_THE_FLAG_GAME_MODE_SYSTEM_H
#define INCLUDED_CORE_CAPTURE_THE_FLAG_GAME_MODE_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "start_game_mode_event.h"
#include "core/program_state.h"

namespace core {

class CaptureTheFlagGameModeSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(CaptureTheFlagGameModeSystem)
    CaptureTheFlagGameModeSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    AutoReg mOnStartGameMode;
    core::ProgramState& mProgramState;
    void OnStartGameMode(core::StartGameModeEvent const& Evt);
};

} // namespace core

#endif//INCLUDED_CORE_CAPTURE_THE_FLAG_GAME_MODE_SYSTEM_H

//command:  "classgenerator.exe" -c "capture_the_flag_game_mode_system" -g "system" -n "core" -e "core-startGameMode"
