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
    ModelValue mGetSelectedLevelModel;
    ModelValue mLevelNamesModel;
    ModelValue mLevelImagesModel;
    std::string mSelectedLevel;
    std::vector<std::string> mLevelNames;
    std::vector<std::string> mLevelImages;
public:
    DEFINE_SYSTEM_BASE(LevelSelectionSystem)
    LevelSelectionSystem();
    std::string GetSelectedLevel();
    void SelectLevel( std::string const& Level );
    std::vector<std::string> GetLevelNames();
    std::vector<std::string> GetLevelImages();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
};

} // namespace core

#endif//INCLUDED_CORE_LEVEL_SELECTION_SYSTEM_H
//command:  "./classgenerator" -c "level_selection_system" -g "system" -n "core"
