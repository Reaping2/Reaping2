#include "platform/i_platform.h"
#include "rogue_game_mode_system.h"
#include "engine/soldier_spawn_system.h"
#include "engine/ctf_soldier_spawn_system.h"
#include "map/editor_system.h"
#include "map/editor_soldier_spawn_system.h"
#include "capture_the_flag_game_mode_system.h"
#include "core/free_for_all_game_mode_system.h"
#include "engine/engine.h"
#include "ui/ui.h"

namespace core {

RogueGameModeSystem::RogueGameModeSystem()
    : mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
{
}


void RogueGameModeSystem::Init()
{
    mOnStartGameMode = EventServer<core::StartGameModeEvent>::Get().Subscribe( boost::bind( &RogueGameModeSystem::OnStartGameMode, this, _1 ) );
    mOnLevelSelected = EventServer<core::LevelSelectedEvent>::Get().Subscribe( boost::bind( &RogueGameModeSystem::OnLevelSelected, this, _1 ) );
}


void RogueGameModeSystem::Update(double DeltaTime)
{
}


void RogueGameModeSystem::OnStartGameMode( core::StartGameModeEvent const& Evt )
{
    if (Evt.mMode != "rogue")
    {
        return;
    }
    ::engine::Engine::Get().SetEnabled< ::engine::SoldierSpawnSystem>( true );
    ::engine::Engine::Get().SetEnabled< ::engine::ctf::CtfSoldierSpawnSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::EditorSoldierSpawnSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::EditorSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::core::FreeForAllGameModeSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::core::CaptureTheFlagGameModeSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::core::RogueGameModeSystem>( true );

    mScene.Load( mScene.GetSelectedLevel() );
    //    glfwSetInputMode(engine::Engine::Get().GetSystem<engine::WindowSystem>()->GetWindow(),GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
    Ui::Get().Load( "hud" );
    if (ProgramState::Get().mMode == ProgramState::Client)
    {
        return;
    }

}

void RogueGameModeSystem::OnLevelSelected( core::LevelSelectedEvent const& Evt )
{
    if ("rogue" != mProgramState.mGameMode)
    {
        return;
    }
    // the host did the last step in config, redirect it to the client list
    Ui::Get().Load( "ffa_client_list" );
}

} // namespace engine

