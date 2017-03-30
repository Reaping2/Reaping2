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
#include "map/editor_target_system.h"
#include "map/editor_grid_system.h"
#include "map/editor_brush_system.h"
#include "map/room_editor_system.h"
#include "level_selection_system.h"
#include "i_health_component.h"
#include "health_component.h"
#include "i_inventory_component.h"
#include "buffs/i_buff_holder_component.h"
#include "i_move_component.h"
#include "engine/system_suppressor.h"

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
    mOnMapStart = EventServer<core::MapStartEvent>::Get().Subscribe( boost::bind( &RogueGameModeSystem::OnMapStart, this, _1 ) );
    mOnMapLoad = EventServer<core::MapLoadEvent>::Get().Subscribe( boost::bind( &RogueGameModeSystem::OnMapLoad, this, _1 ) );
    mOnSoldierCreated = EventServer<engine::SoldierCreatedEvent>::Get().Subscribe( boost::bind( &RogueGameModeSystem::OnSoldierCreated, this, _1 ) );
}


void RogueGameModeSystem::Update(double DeltaTime)
{
}


void RogueGameModeSystem::OnStartGameMode( core::StartGameModeEvent const& Evt )
{
    if (Evt.mMode != GameModes::Rogue )
    {
        return;
    }
    ::engine::Engine::Get().SetEnabled< ::engine::SoldierSpawnSystem>( true );
    ::engine::Engine::Get().SetEnabled< ::engine::ctf::CtfSoldierSpawnSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::EditorSoldierSpawnSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::RoomEditorSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::EditorSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::EditorTargetSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::EditorGridSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::map::EditorBrushSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::core::FreeForAllGameModeSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::core::CaptureTheFlagGameModeSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::core::RogueGameModeSystem>( true );
    if (ProgramState::Get().mMode == ProgramState::Client)
    {
        return;
    }
    auto levelSelectionSystem = engine::Engine::Get().GetSystem<LevelSelectionSystem>();
    if ( levelSelectionSystem.IsValid() )
    {
        mScene.Load( levelSelectionSystem->GetSelectedLevel() );
    }
    else
    {
        L1("failed to retrieve LevelSelectionSystem\n");
        return;
    }

}

void RogueGameModeSystem::OnLevelSelected( core::LevelSelectedEvent const& Evt )
{
    if ( GameModes::Rogue != mProgramState.mGameMode)
    {
        return;
    }
    // the host did the last step in config, redirect it to the client list
    Ui::Get().Load( "ffa_client_list" );
}

void RogueGameModeSystem::OnMapStart( core::MapStartEvent const& Evt )
{
    if (GameModes::Rogue != mProgramState.mGameMode)
    {
        return;
    }
    if (Evt.mState == core::MapStartEvent::Ready)
    {
        mProgramState.mHUD = "hud";
        Ui::Get().Load( mProgramState.mHUD );
    }
    if (Evt.mState == core::MapStartEvent::ActorsSpawned&&mProgramState.mMode != ProgramState::Client)
    {
        bool succ = engine::SystemSuppressor::Get().Resume( engine::SystemSuppressor::SceneLoad );
    }
}

void RogueGameModeSystem::OnMapLoad( core::MapLoadEvent const& Evt )
{
    L2( "Map Load on game Mode:%d", mProgramState.mGameMode );
    if (GameModes::Rogue != mProgramState.mGameMode)
    {
        return;
    }
    L2( "Map Load rogue game Mode!" );
    if (ProgramState::Get().mMode != ProgramState::Client)
    {
        mComponentMap.clear();
        for (auto& clientData : mProgramState.mClientDatas)
        {
            auto& components = mComponentMap[clientData.mClientId];
            auto player( mScene.GetActor( clientData.mClientActorGUID ) );
            if (player.IsValid())
            {
                components.AddComponent(
                    std::auto_ptr<Component>( Clone( player->Get<IHealthComponent>() ) ) );
                components.AddComponent(
                    std::auto_ptr<Component>( Clone( player->Get<IInventoryComponent>() ) ) );
                components.AddComponent(
                    std::auto_ptr<Component>( Clone( player->Get<IBuffHolderComponent>() ) ) );
                components.AddComponent(
                    std::auto_ptr<Component>( Clone( player->Get<IMoveComponent>() ) ) );
            }
        }
    }
    Ui::Get().Load( "waiting_load" );
    bool succ = engine::SystemSuppressor::Get().Suppress( engine::SystemSuppressor::SceneLoad );
}

void RogueGameModeSystem::OnSoldierCreated( engine::SoldierCreatedEvent const& Evt )
{
    if (Evt.mState != engine::SoldierCreatedEvent::PropertiesSet)
    {
        return;
    }
    auto it = mComponentMap.find(Evt.mClientData.mClientId);
    if (it == mComponentMap.end())
    {
        return;
    }
    auto& components = it->second;
    auto player = Evt.mActor;
  
    for (auto component : components.GetComponents())
    {
        player->AddComponent( std::auto_ptr<Component>( component.second ) );
    }


    components.Clear();
    mComponentMap.erase( it );
}


ComponentHolder::ComponentList_t& RogueGameModeSystem::Components::GetComponents()
{
    return mComponents;
}

void RogueGameModeSystem::Components::Clear( bool deleteComps /*= false */ )
{
    if (deleteComps)
    {
        for (auto i : mComponents)
        {
            delete i.second;
        }
    }
    mComponents.clear();
}

} // namespace engine

