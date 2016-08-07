#ifndef INCLUDED_MAP_EDITOR_GROUP_SYSTEM_H
#define INCLUDED_MAP_EDITOR_GROUP_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "editor_mode_changed_event.h"
#include "editor_back_event.h"
#include "level_generator/room_desc.h"
#include "input/mouse.h"
#include "map_element_removed_event.h"

namespace map {

class EditorGroupSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(EditorGroupSystem)
    EditorGroupSystem();
    static Opt<EditorGroupSystem> Get();
    std::vector<std::string> GetGroupNames() const;
    void SetGroupNames( std::vector<std::string> val );
    static int32_t RemoveFromAllGroups( int32_t actorGUID );

    static void SetMapElementIdentifier( int32_t mapElementUID, int32_t spawnID );

    static int32_t RemoveFromGroup( int32_t actorGUID, int32_t groupId );
    static void AddToGroup( std::vector<int32_t> actorGUIDs, int32_t groupId );
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
    AutoReg mOnMapElementRemoved;
    void OnMapElementRemoved( map::MapElementRemovedEvent const& Evt );
    ModelValue mEditorGroupModel;
    ModelValue mNewGroupModel;
    void OnNewGroup();
    ModelValue mGroupNamesModel;
    std::vector<std::string> mGroupNames;
    ModelValue mGroupSelectedModel;
    void OnGroupSelected( std::string groupName );
};

} // namespace map

#endif//INCLUDED_MAP_EDITOR_GROUP_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -n "map" -c "editor_actor_system" -e "map-editorModeChanged"
