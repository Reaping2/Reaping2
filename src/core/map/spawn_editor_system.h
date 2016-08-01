#ifndef INCLUDED_MAP_SPAWN_SYSTEM_H
#define INCLUDED_MAP_SPAWN_SYSTEM_H

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

namespace map {

class SpawnEditorSystem : public PropertyEditorBaseSystem<SpawnEditorSystem>
{
public:
    DEFINE_SYSTEM_BASE(SpawnEditorSystem)
    SpawnEditorSystem();
    static Opt<SpawnEditorSystem> Get();
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
    std::vector<std::string> mPropNames;
    ModelValue mPropSelectedModel;
    void OnPropSelected( std::string groupName );

    void LoadProp();

    Opt<RoomDesc> mRoomDesc;
    ModelValue mTargetNameModel;
    std::string mTargetName;
    AutoReg mOnRoomEditorLoaded;
    int32_t mPropUID = -1;
    ModelValue mChangeTargetModel;
    void OnChangeTarget();
    AutoReg mOnGroupSelected;
    ActorColors_t mGroupPreSelectedActors;
    std::string mGroupPreSelectName;
    void OnGroupSelected( map::GroupSelectedEvent const& Evt );
    void OnRoomEditorLoaded( map::RoomEditorLoadedEvent const& Evt );

    void LoadRoomDesc();

    int32_t mChance = 0;
    ModelValue mChancePressModel;
    ModelValue mChanceModel;
    void ChancePress( std::string modifier );
};

} // namespace map

#endif//INCLUDED_MAP_SPAWN_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -n "map" -c "editor_actor_system" -e "map-editorModeChanged"
