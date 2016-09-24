#ifndef INCLUDED_MAP_EDITOR_ACTOR_SYSTEM_H
#define INCLUDED_MAP_EDITOR_ACTOR_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "editor_mode_changed_event.h"
#include "editor_back_event.h"

namespace map {

class EditorActorSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(EditorActorSystem)
    EditorActorSystem();
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
};

} // namespace map

#endif//INCLUDED_MAP_EDITOR_ACTOR_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -n "map" -c "editor_actor_system" -e "map-editorModeChanged"
