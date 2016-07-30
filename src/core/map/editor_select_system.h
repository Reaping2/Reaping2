#ifndef INCLUDED_MAP_EDITOR_SELECT_SYSTEM_H
#define INCLUDED_MAP_EDITOR_SELECT_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "editor_mode_changed_event.h"
#include "editor_back_event.h"
#include "level_generator/room_desc.h"
#include "input/mouse.h"
#include "group_selected_event.h"

namespace map {

typedef std::map<int32_t, glm::vec4> ActorColors_t;

class EditorSelectSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(EditorSelectSystem)
    EditorSelectSystem();
    static Opt<EditorSelectSystem> Get();
    static void SetActorColors( ActorColors_t const& actorGUIDs, Opt<glm::vec4> col );
    static void AddToActorColors( int32_t actorGUID, ActorColors_t &actorColors, Opt<ActorColors_t> colorShaders = nullptr );
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
    ModelValue mEditorSelectModel;
    glm::vec2 mMousePos;
    glm::vec2 mSelectStartPos;
    glm::vec2 mSelectEndPos;
    ActorColors_t mSelectedActors;
    ActorColors_t mCurrentSelectedActors;
    ActorColors_t mGroupPreSelectedActors;
    glm::vec4 mSelectColor = glm::vec4( 0, 0, 0.8, 1 );
    glm::vec4 mPreSelectColor = glm::vec4( 0.8, 0, 0.8, 1 );
    AutoReg mMouseMoveId;
    bool mMouseLeftPressed = false;
    bool mMouseRightPressed = false;
    bool mSelectStarted = false;
    std::string mGroupPreSelectName;
    void OnMouseMoveEvent( const WorldMouseMoveEvent& Event );
    ModelValue mMouseModeModel;
    void SetMouseMode( std::string mode );
    ModelValue mEditorSelectStateModel;
    void SetEditorSelectState( std::string state );
    void UpdateSelectedActors();
    AutoReg mOnGroupSelected;
    void OnGroupSelected( map::GroupSelectedEvent const& Evt );
    void SetUIDUniqueForSelectedActors();
    enum SelectState
    {
        AddToGroup,
        RemoveFromGroup
    } mSelectState = AddToGroup;
    ModelValue mRemoveFromAllGroupsModel;
    void OnRemoveFromAllGroups();

};

} // namespace map

#endif//INCLUDED_MAP_EDITOR_SELECT_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -n "map" -c "editor_actor_system" -e "map-editorModeChanged"
