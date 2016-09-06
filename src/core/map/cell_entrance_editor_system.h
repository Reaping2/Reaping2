#ifndef INCLUDED_MAP_CELL_ENTRANCE_EDITOR_SYSTEM_H
#define INCLUDED_MAP_CELL_ENTRANCE_EDITOR_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "editor_mode_changed_event.h"
#include "editor_back_event.h"
#include "level_generator/room_desc.h"
#include "input/mouse.h"
#include "room_editor_loaded_event.h"
#include "editor_select_system.h"
#include "group_selected_event.h"
#include "property_editor_base_system.h"
#include "level_generator/cell_entrance_property.h"

namespace map {

class CellEntranceEditorSystem : public PropertyEditorBaseSystem<CellEntranceEditorSystem, CellEntranceProperty>
{
public:
    DEFINE_SYSTEM_BASE(CellEntranceEditorSystem)
    CellEntranceEditorSystem();
    static Opt<CellEntranceEditorSystem> Get();
    int32_t GetPropUIDFromScene( glm::vec2 mousePos );
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );

    int32_t AddNewProperty();

private:
    Scene& mScene;
    AutoReg mOnEditorModeChanged;
    void OnEditorModeChanged(map::EditorModeChangedEvent const& Evt);
    void EnableSubsystems( bool enable );
    AutoReg mOnEditorBack;
    void OnEditorBack( map::EditorBackEvent const& Evt );
    ModelValue mEditorPropModel;
    ModelValue mNewPropModel;
    void OnNewProp();
    ModelValue mPropNamesModel;
    ModelValue mPropSelectedModel;
    void OnPropSelected( std::string groupName );
    void LoadProp();
    int32_t mPropUID = -1;
    ModelValue mTargetNameModel;
    std::string mTargetName;
    ModelValue mChangeTargetModel;
    void OnChangeTarget();
    ModelValue mBlockedTargetNameModel;
    std::string mBlockedTargetName;
    ModelValue mChangeBlockedTargetModel;
    void OnChangeBlockedTarget();
    AutoReg mOnGroupSelected;
    ActorColors_t mGroupPreSelectedActors;
    std::string mGroupPreSelectName;
    void OnGroupSelected( map::GroupSelectedEvent const& Evt );
    glm::vec2 mMousePos;
    AutoReg mMouseMoveId;
    void OnMouseMoveEvent( const WorldMouseMoveEvent& Event );
    enum Mode
    {
        EntranceTarget,
        BlockedTarget,
        PropSelect,
        Edit
    } mEditorMode;
};

} // namespace map

#endif//INCLUDED_MAP_CELL_ENTRANCE_EDITOR_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -n "map" -c "editor_actor_system" -e "map-editorModeChanged"
