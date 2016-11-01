#ifndef INCLUDED_CORE_LEVEL_SELECTION_SYSTEM_H
#define INCLUDED_CORE_LEVEL_SELECTION_SYSTEM_H

#include "platform/model_value.h"
#include "engine/system.h"
using platform::ModelValue;

namespace core {

class LevelSelectionSystem : public engine::System
{
    ModelValue mLevelModel;
    ModelValue mSelectLevelModel;
    ModelValue mLevelDisplayNamesModel;
    ModelValue mLevelThumbnailsModel;
    std::string mSelectedLevel;
    std::vector<std::string> mLevelDisplayNames;
    std::vector<std::string> mLevelRealNames;
    std::vector<int32_t> mLevelThumbnails;
public:
    DEFINE_SYSTEM_BASE(LevelSelectionSystem)
    LevelSelectionSystem();
    void SelectLevelByIdx( int32_t idx );
    void SelectLevelByName( std::string realName );
    std::string GetSelectedLevel();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
};

} // namespace core

#endif//INCLUDED_CORE_LEVEL_SELECTION_SYSTEM_H
//command:  "./classgenerator" -c "level_selection_system" -g "system" -n "core"
