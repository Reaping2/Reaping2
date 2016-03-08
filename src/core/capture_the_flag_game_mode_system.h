#ifndef INCLUDED_CORE_CAPTURE_THE_FLAG_GAME_MODE_SYSTEM_H
#define INCLUDED_CORE_CAPTURE_THE_FLAG_GAME_MODE_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "start_game_mode_event.h"
#include "core/program_state.h"
#include "ctf_program_state.h"
#include "flag_state_changed_event.h"
#include "engine/score_event.h"
#include "input/input_system.h"
#include "level_selected_event.h"

namespace core {
using platform::ModelValue;

class CaptureTheFlagGameModeSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE( CaptureTheFlagGameModeSystem )
    CaptureTheFlagGameModeSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    AutoReg mOnStartGameMode;
    AutoReg mOnLevelSelected;
    core::ProgramState& mProgramState;
    ctf::ProgramState& mCtfProgramState;
    Opt<engine::InputSystem> mInputSystem;
    bool mHudShown;
    ModelValue mCtfModel;
    boost::ptr_vector<ModelValue> mTeamModels;
    void OnStartGameMode( core::StartGameModeEvent const& Evt );
    void OnLevelSelected( core::LevelSelectedEvent const& Evt );
    AutoReg mOnFlagStateChanged;
    void OnFlagStateChanged( ctf::FlagStateChangedEvent const& Evt );
    AutoReg mOnScore;
    void OnScore( engine::ScoreEvent const& Evt );
};

} // namespace core

#endif//INCLUDED_CORE_CAPTURE_THE_FLAG_GAME_MODE_SYSTEM_H

//command:  "classgenerator.exe" -c "capture_the_flag_game_mode_system" -g "system" -n "core" -e "core-startGameMode"
