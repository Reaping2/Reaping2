#ifndef INCLUDED_MAP_ROOM_PLAIN_PROPERTY_EDITOR_SYSTEM_H
#define INCLUDED_MAP_ROOM_PLAIN_PROPERTY_EDITOR_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "editor_mode_changed_event.h"
#include "editor_back_event.h"
#include "level_generator/room_desc.h"
#include "input/mouse.h"
#include "room_editor_loaded_event.h"

namespace map {

class RoomPlainPropertyEditorSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(RoomPlainPropertyEditorSystem)
    RoomPlainPropertyEditorSystem();
    static Opt<RoomPlainPropertyEditorSystem> Get();
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
    ModelValue mPropNamesModel;
    ModelValue mPropStatesModel;
    std::vector<std::string> mPropNames;
    std::vector<std::string> mPropStates;
    ModelValue mPropSelectedModel;
    void OnPropSelected( std::string propName );
    Opt<RoomDesc> mRoomDesc;
    AutoReg mOnRoomEditorLoaded;
    void OnRoomEditorLoaded( map::RoomEditorLoadedEvent const& Evt );

    void LoadPropStates();

};

} // namespace map

#endif//INCLUDED_MAP_ROOM_PLAIN_PROPERTY_EDITOR_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -n "map" -c "editor_actor_system" -e "map-editorModeChanged"
