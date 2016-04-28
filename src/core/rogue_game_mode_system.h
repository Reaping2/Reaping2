#ifndef INCLUDED_ENGINE_ROGUE_GAME_MODE_SYSTEM_H
#define INCLUDED_ENGINE_ROGUE_GAME_MODE_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/start_game_mode_event.h"
#include "core/level_selected_event.h"

namespace core {

class RogueGameModeSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(RogueGameModeSystem)
    RogueGameModeSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    AutoReg mOnStartGameMode;
    AutoReg mOnLevelSelected;
    core::ProgramState& mProgramState;
    void OnStartGameMode( core::StartGameModeEvent const& Evt );
    void OnLevelSelected( core::LevelSelectedEvent const& Evt );
};

} // namespace engine

#endif//INCLUDED_ENGINE_ROGUE_GAME_MODE_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "rogue_game_mode_system"
