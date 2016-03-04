#ifndef INCLUDED_CORE_FREE_FOR_ALL_GAME_MODE_SYSTEM_H
#define INCLUDED_CORE_FREE_FOR_ALL_GAME_MODE_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "start_game_mode_event.h"
#include "level_selected_event.h"

namespace core {

class FreeForAllGameModeSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(FreeForAllGameModeSystem)
    FreeForAllGameModeSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    AutoReg mOnStartGameMode;
    AutoReg mOnLevelSelected;
    void OnStartGameMode(core::StartGameModeEvent const& Evt);
    void OnLevelSelected(core::LevelSelectedEvent const& Evt);
};

} // namespace core

#endif//INCLUDED_CORE_FREE_FOR_ALL_GAME_MODE_SYSTEM_H

//command:  "classgenerator.exe" -c "free_for_all_game_mode_system" -g "system" -n "core" -e "core-startGameMode"
