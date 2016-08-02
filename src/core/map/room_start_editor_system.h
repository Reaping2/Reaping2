#ifndef INCLUDED_MAP_ROOM_START_SYSTEM_H
#define INCLUDED_MAP_ROOM_START_SYSTEM_H

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
#include "level_generator/room_start_property.h"

namespace map {

class RoomStartEditorSystem : public PropertyEditorBaseSystem<RoomStartEditorSystem, RoomStartProperty>
{
public:
    DEFINE_SYSTEM_BASE(RoomStartEditorSystem)
    RoomStartEditorSystem();
    static Opt<RoomStartEditorSystem> Get();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
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
    ModelValue mTargetNameModel;
    std::string mTargetName;
    int32_t mPropUID = -1;
    ModelValue mChangeTargetModel;
    void OnChangeTarget();
    AutoReg mOnGroupSelected;
    ActorColors_t mGroupPreSelectedActors;
    std::string mGroupPreSelectName;
    void OnGroupSelected( map::GroupSelectedEvent const& Evt );
};

} // namespace map

#endif//INCLUDED_MAP_ROOM_START_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -n "map" -c "editor_actor_system" -e "map-editorModeChanged"
