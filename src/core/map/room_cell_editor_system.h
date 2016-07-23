#ifndef INCLUDED_MAP_ROOM_CELL_EDITOR_SYSTEM_H
#define INCLUDED_MAP_ROOM_CELL_EDITOR_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "editor_mode_changed_event.h"
#include "editor_back_event.h"
#include "level_generator/room_desc.h"

namespace map {

class RoomCellEditorSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(RoomCellEditorSystem)
    RoomCellEditorSystem();
    static Opt<RoomCellEditorSystem> Get();
    void SetRoomDesc( Opt<RoomDesc> roomDesc );
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
    void RemoveCells();
    void AddCells();
    Opt<RoomDesc> mRoomDesc;
    std::vector<int32_t> mCellGUIDs;
};

} // namespace map

#endif//INCLUDED_MAP_ROOM_CELL_EDITOR_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -n "map" -c "editor_actor_system" -e "map-editorModeChanged"
