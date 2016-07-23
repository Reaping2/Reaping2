#include "platform/i_platform.h"
#include "editor_layer_system.h"
#include <boost/assign/std/vector.hpp>
#include "editor_back_event.h"
#include "../../engine/engine.h"
namespace map {

EditorLayerSystem::EditorLayerSystem()
    : mScene( Scene::Get() )
    , mEditorLayer( EditorLayer::Get() )
{
}


void EditorLayerSystem::Init()
{
    ModelValue& editorModel = const_cast<ModelValue&>(RootModel::Get()["editor"]);
    std::auto_ptr<ModelValue> layerModel( new ModelValue( StringFunc( this, &EditorLayerSystem::LayerSelect ), "layer", &editorModel ) );
    mEditorModels.push_back( new ModelValue( (ModelValue::get_string_vec_t) boost::bind( &EditorLayerSystem::LayerNames, this ), "names", layerModel.get() ) );


    mEditorModels.push_back( layerModel );
    using namespace boost::assign;
    mLayerNames += "any", "target";
}


void EditorLayerSystem::Update(double DeltaTime)
{
}

std::vector<std::string> EditorLayerSystem::LayerNames()
{
    return mLayerNames;
}

void EditorLayerSystem::LayerSelect( std::string const& layer )
{
    mEditorLayerType = mEditorLayer( IdStorage::Get().GetId( layer ) );
    EventServer<EditorBackEvent>::Get().SendEvent( EditorBackEvent() );
}

Opt<EditorLayerSystem> EditorLayerSystem::Get()
{
    return engine::Engine::Get().GetSystem<EditorLayerSystem>();
}

map::EditorLayer::Type EditorLayerSystem::GetEditorLayerType() const
{
    return mEditorLayerType;
}

} // namespace map

