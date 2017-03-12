#ifndef INCLUDED_CORE_LEVEL_SELECTION_SYSTEM_H
#define INCLUDED_CORE_LEVEL_SELECTION_SYSTEM_H

#include "platform/model_value.h"
#include "engine/system.h"
#include "core/gamemode_selected_event.h"
#include "core/game_modes.h"

using platform::ModelValue;
using platform::AutoReg;

namespace core {

class LevelSelectionSystem : public engine::System
{
    ModelValue mLevelModel;
    ModelValue mSelectLevelModel;
    ModelValue mSelectLevelByNameModel;
    ModelValue mLevelDisplayNamesModel;
    ModelValue mLevelThumbnailsModel;
    std::string mSelectedLevel;
    // caches selected game mode from ProgramState
    GameModes::Type mGameMode;
    // mappings
    // game mode to display names of available maps
    std::map<GameModes::Type,std::vector<std::string>> mLevelDisplayNames;
    // game mode to real names of available maps
    std::map<GameModes::Type ,std::vector<std::string>> mLevelRealNames;
    // game mode to thimbnail id of available maps
    std::map<GameModes::Type ,std::vector<int32_t>> mLevelThumbnails;
public:
    DEFINE_SYSTEM_BASE(LevelSelectionSystem)
    LevelSelectionSystem();
    // used by the UI
    void SelectLevelByIdx( int32_t idx );
    // used by the UI
    void SelectLevelByNameUI( std::string const& levelName );
    // used by other other Systems
    void SelectLevelByName( core::GameModes::Type gameMode, std::string const& realName );
    std::string GetSelectedLevel();

    std::vector<std::string> DisplayNames();
    std::vector<int32_t> Thumbnails();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    AutoReg mOnGamemodeSelectedEvent;
    void OnGamemodeSelectedEvent( core::GamemodeSelectedEvent const& evt );
};

} // namespace core

#endif//INCLUDED_CORE_LEVEL_SELECTION_SYSTEM_H
//command:  "./classgenerator" -c "level_selection_system" -g "system" -n "core"
