#include "platform/i_platform.h"
#include "editor_grid_system.h"
#include "engine/engine.h"
#include "editor_target_system.h"
#include "ui/ui.h"

namespace map {

EditorGridSystem::EditorGridSystem()
    : mScene( Scene::Get() )
    , mGridRepo( GridRepo::Get() )
    , mGridId( -1 )
{
}


void EditorGridSystem::Init()
{
    ModelValue& editorModel = const_cast<ModelValue&>( RootModel::Get()["editor"] );
    mEditorModels.push_back( new ModelValue( StringFunc( this, &EditorGridSystem::GridChanged ), "grid", &editorModel ) );
    mOnWorldMouseMove = EventServer< ::WorldMouseMoveEvent>::Get().Subscribe( boost::bind( &EditorGridSystem::OnWorldMouseMoveEvent, this, _1 ) );
    mGridId = AutoId( "matrix" );
}


void EditorGridSystem::Update( double DeltaTime )
{
    EditorTargetSystem::Get()->SetCursorPosition( GetGrid().GetProcessedPosition().x, GetGrid().GetProcessedPosition().y );
}

void EditorGridSystem::GridChanged( std::string const& grid )
{
    mGridId = AutoId( grid );
    Ui::Get().Load( "editor_hud" );
}

EditorGridSystem::~EditorGridSystem()
{
    mEditorModels.clear();
}

Opt<EditorGridSystem> EditorGridSystem::Get()
{
    return engine::Engine::Get().GetSystem<EditorGridSystem>();
}

IGrid& EditorGridSystem::GetGrid()
{
    return mGridRepo( mGridId );
}

void EditorGridSystem::OnWorldMouseMoveEvent( ::WorldMouseMoveEvent const& Evt )
{
    GetGrid().SetMousePosition( Evt.Pos.x, Evt.Pos.y );
}



} // namespace map

