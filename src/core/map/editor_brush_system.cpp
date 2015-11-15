#include "platform/i_platform.h"
#include "editor_brush_system.h"
#include "engine/engine.h"

namespace map {

EditorBrushSystem::EditorBrushSystem()
    : mScene( Scene::Get() )
    , mBrushRepo(BrushRepo::Get())
    , mBrushId(-1)
{
}


void EditorBrushSystem::Init()
{
    ModelValue& editorModel = const_cast<ModelValue&>(RootModel::Get()["editor"]);
    mEditorModels.push_back(new ModelValue( StringFunc(this,&EditorBrushSystem::BrushChanged),"brush",&editorModel));

}


void EditorBrushSystem::Update(double DeltaTime)
{
    GetBrush().Update(DeltaTime);
}

void EditorBrushSystem::BrushChanged(std::string const& brush)
{
    mBrushId=AutoId(brush);
}

EditorBrushSystem::~EditorBrushSystem()
{
    mEditorModels.clear();
}

Opt<EditorBrushSystem> EditorBrushSystem::Get()
{
    return engine::Engine::Get().GetSystem<EditorBrushSystem>();
}

IBrush& EditorBrushSystem::GetBrush()
{
    return mBrushRepo(mBrushId);
}


} // namespace map

