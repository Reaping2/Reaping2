#include "platform/i_platform.h"
#include "editor_renderable_layer_system.h"
#include "ui/ui.h"
#include "editor_grid_system.h"
#include "editor_target_system.h"
#include "editor_brush_system.h"
#include "../../engine/engine.h"
#include "../../input/keyboard.h"
#include "editor_hud_state.h"
#include "input/mouse.h"
#include "layer_selected_event.h"
#include "map_system.h"
#include "../renderable_layer.h"
#include "../../platform/id_storage.h"

namespace map {

EditorRenderableLayerSystem::EditorRenderableLayerSystem()
    : mScene( Scene::Get() )
    , mEditorLayerModel( "editor_layer", &RootModel::Get() )
    , mLayerNamesModel( (ModelValue::get_string_vec_t) RefTo( mLayerNames ), "layers", &mEditorLayerModel )
    , mLayerSelectedModel( StringFunc( this, &EditorRenderableLayerSystem::OnLayerSelected ), "layer_selected", &mEditorLayerModel )
{
}


void EditorRenderableLayerSystem::Init()
{
    mOnEditorModeChanged = EventServer<map::EditorModeChangedEvent>::Get().Subscribe( boost::bind( &EditorRenderableLayerSystem::OnEditorModeChanged, this, _1 ) );
    mOnEditorBack = EventServer<map::EditorBackEvent>::Get().Subscribe( boost::bind( &EditorRenderableLayerSystem::OnEditorBack, this, _1 ) );
    auto& idStorage = IdStorage::Get();
    for (auto idToLayer : RenderableLayer::Get().GetIdToRenderLayerMap())
    {
        std::string name;
        if (idStorage.GetName( idToLayer.first, name ))
        {
            mLayerNames.push_back( name );
        }
    }
}


void EditorRenderableLayerSystem::Update(double DeltaTime)
{
    if (!mEnabled)
    {
        return;
    }
    static Opt<engine::KeyboardSystem> keyboard = ::engine::Engine::Get().GetSystem<engine::KeyboardSystem>();
    static Opt<engine::MouseSystem> mouse = ::engine::Engine::Get().GetSystem<MouseSystem>();
}

void EditorRenderableLayerSystem::OnEditorModeChanged(map::EditorModeChangedEvent const& Evt)
{
}

void EditorRenderableLayerSystem::OnEditorBack( map::EditorBackEvent const& Evt )
{
}

void EditorRenderableLayerSystem::EnableSubsystems( bool enable )
{
}

Opt<EditorRenderableLayerSystem> EditorRenderableLayerSystem::Get()
{
    return engine::Engine::Get().GetSystem<EditorRenderableLayerSystem>();
}

void EditorRenderableLayerSystem::OnLayerSelected( std::string layerName )
{
    EventServer<LayerSelectedEvent>::Get().SendEvent( LayerSelectedEvent( layerName ) );
}

} // namespace map

