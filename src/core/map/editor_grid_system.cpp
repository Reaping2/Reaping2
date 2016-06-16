#include "platform/i_platform.h"
#include "editor_grid_system.h"
#include "engine/engine.h"
#include "editor_target_system.h"
#include "ui/ui.h"
#include <boost/assign/std/vector.hpp>

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
    ModelValue& gridModel = mEditorModels.back();
    mEditorModels.push_back( new ModelValue( (ModelValue::get_string_vec_t) boost::bind( &EditorGridSystem::GridStyles, this ), "styles", &gridModel ) );
    mOnWorldMouseMove = EventServer< ::WorldMouseMoveEvent>::Get().Subscribe( boost::bind( &EditorGridSystem::OnWorldMouseMoveEvent, this, _1 ) );
    mGridId = AutoId( "matrix" );
    using namespace boost::assign;
    mGridStyles += "matrix","absolute";
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

std::vector<std::string> EditorGridSystem::GridStyles()
{
    return mGridStyles;
}

} // namespace map

