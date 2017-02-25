#ifndef INCLUDED_CORE_FREE_FOR_ALL_GAME_MODE_SYSTEM_H
#define INCLUDED_CORE_FREE_FOR_ALL_GAME_MODE_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "start_game_mode_event.h"
#include "level_selected_event.h"
#include "program_state.h"
#include "map_start_event.h"
#include "map_load_event.h"

namespace core {

class FreeForAllGameModeSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE( FreeForAllGameModeSystem )
    FreeForAllGameModeSystem();
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
    AutoReg mOnMapStart;
    void OnMapStart( core::MapStartEvent const& Evt );
    AutoReg mOnMapLoad;
    void OnMapLoad( core::MapLoadEvent const& Evt );
};

} // namespace core

#endif//INCLUDED_CORE_FREE_FOR_ALL_GAME_MODE_SYSTEM_H

//command:  "classgenerator.exe" -c "free_for_all_game_mode_system" -g "system" -n "core" -e "core-startGameMode"
