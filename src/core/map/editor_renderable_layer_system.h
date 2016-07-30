#ifndef INCLUDED_MAP_EDITOR_RENDERABLE_LAYER_SYSTEM_H
#define INCLUDED_MAP_EDITOR_RENDERABLE_LAYER_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "editor_mode_changed_event.h"
#include "editor_back_event.h"
#include "level_generator/room_desc.h"
#include "input/mouse.h"

namespace map {

class EditorRenderableLayerSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(EditorRenderableLayerSystem)
    EditorRenderableLayerSystem();
    static Opt<EditorRenderableLayerSystem> Get();
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
    ModelValue mEditorLayerModel;
    ModelValue mLayerNamesModel;
    std::vector<std::string> mLayerNames;
    ModelValue mLayerSelectedModel;
    void OnLayerSelected( std::string groupName );
};

} // namespace map

#endif//INCLUDED_MAP_EDITOR_RENDERABLE_LAYER_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -n "map" -c "editor_actor_system" -e "map-editorModeChanged"
