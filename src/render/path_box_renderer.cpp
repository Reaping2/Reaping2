#include "i_render.h"
#include "path_box_renderer.h"
#include "core/i_position_component.h"
#include "core/i_inventory_component.h"
#include "core/i_health_component.h"
#include "core/i_collision_component.h"
#include "core/i_renderable_component.h"
#include "core/actor.h"
#include "recognizer.h"
#include "main/window.h"
#include "engine/engine.h"
#include "text.h"
#include "text_uimodel.h"
#include "core/i_team_component.h"
#include "engine/cloak_system.h"
#include "engine/path_system.h"
#include "core/program_state.h"

void PathBoxRenderer::Init()
{
    mSize = mSettings.GetInt("find_path.box_name.size", 76);
}

PathBoxRenderer::PathBoxRenderer()
    : mProgramState( core::ProgramState::Get() )
    , mColorRepo( render::ColorRepo::Get() )
    , mSettings( Settings::Get() )
    , mSize(76)
{
    Init();
}

void PathBoxRenderer::Draw( TextSceneRenderer& textSceneRenderer )
{
    static const auto mDebugPathSystem = Settings::Get().GetInt( "find_path.debug", 0 )!=0;
    if (!mDebugPathSystem)
    {
        return;
    }
    static auto pathSystem = engine::Engine::Get().GetSystem<engine::path::PathSystem>();
    auto& pathGraph = pathSystem->GetGraph();
    auto actor( Scene::Get().GetActor( mProgramState.mControlledActorGUID ) );
    if (!actor.IsValid())
    {
        return;
    }
    //auto positionC( actor->Get<IPositionComponent>() );
    int32_t boxInd = pathGraph.GetBoxIndex( *actor );
    auto const& nodeDistance = pathSystem->GetNodeDistance( mProgramState.mControlledActorGUID );
    for (int32_t i = 0; i < nodeDistance.size(); ++i)
    {
        auto center = pathGraph.GetBoxCenter( i );
        Text text( mSize, glm::vec4( center, 500, 500 ),
                   glm::vec4( 1.0, 1.0, 1.0, 1.0 )
                   , boost::lexical_cast<std::string>(int32_t( nodeDistance[i])), true );
        textSceneRenderer.AddText( text );
    }
}


