#ifndef INCLUDED_MAP_EDITOR_VISIBLITY_SYSTEM_H
#define INCLUDED_MAP_EDITOR_VISIBLITY_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "editor_mode_changed_event.h"
#include "editor_back_event.h"
#include "level_generator/room_desc.h"
#include "input/mouse.h"
#include "layer_selected_event.h"
#include "group_selected_event.h"
#include "editor_select_system.h"
#include "groups_changed_event.h"

namespace map {

class EditorVisibilitySystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(EditorVisibilitySystem)
    EditorVisibilitySystem();
    static Opt<EditorVisibilitySystem> Get();
    static glm::vec4 InvisibleColor;
    void UpdateInvisibleActors();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    AutoReg mOnEditorModeChanged;
    ActorColors_t mInvisibleActors;
    std::set<int32_t> mInvisibleGroups;
    std::set<int32_t> mInvisibleLayers;
    void OnEditorModeChanged(map::EditorModeChangedEvent const& Evt);
    void EnableSubsystems( bool enable );
    AutoReg mOnEditorBack;
    void OnEditorBack( map::EditorBackEvent const& Evt );
    AutoReg mOnLayerSelected;
    void OnLayerSelected( map::LayerSelectedEvent const& Evt );
    AutoReg mOnGroupSelected;
    void OnGroupSelected( map::GroupSelectedEvent const& Evt );
    AutoReg mOnGroupsChanged;
    void OnGroupsChanged( map::GroupsChangedEvent const& Evt );
    ModelValue mEditorVisibilityModel;
    ModelValue mShowAllModel;
    void OnShowAll();
    ModelValue mSwitchCellsModel;
    void OnSwitchCells();
};

} // namespace map

#endif//INCLUDED_MAP_EDITOR_VISIBLITY_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -n "map" -c "editor_actor_system" -e "map-editorModeChanged"
