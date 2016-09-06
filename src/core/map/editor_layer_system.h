#ifndef INCLUDED_MAP_EDITOR_LAYER_SYSTEM_H
#define INCLUDED_MAP_EDITOR_LAYER_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "editor_layer.h"

namespace map {

class EditorLayerSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE(EditorLayerSystem)
    EditorLayerSystem();
    static Opt<EditorLayerSystem> Get();
    EditorLayer::Type GetEditorLayerType() const;
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    boost::ptr_vector<ModelValue> mEditorModels;
    
    EditorLayer::Type mEditorLayerType;
    EditorLayer& mEditorLayer;
    std::vector<std::string> mLayerNames;
    std::vector<std::string> LayerNames();
    void LayerSelect( std::string const& layer );
};

} // namespace map

#endif//INCLUDED_MAP_EDITOR_LAYER_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -n "map" -c "editor_layer_system"
