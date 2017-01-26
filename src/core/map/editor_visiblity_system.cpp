#include "platform/i_platform.h"
#include "editor_visiblity_system.h"
#include "ui/ui.h"
#include "editor_grid_system.h"
#include "editor_target_system.h"
#include "editor_brush_system.h"
#include "../../engine/engine.h"
#include "../../input/keyboard.h"
#include "editor_hud_state.h"
#include "input/mouse.h"
#include "group_selected_event.h"
#include "group_map_element.h"
#include "map_system.h"
#include "../renderable_layer.h"
#include "../i_renderable_component.h"
#include "room_cell_editor_system.h"

namespace map {

    glm::vec4 EditorVisibilitySystem::InvisibleColor = glm::vec4( 1.0, 1.0, 1.0, 0.3 );

EditorVisibilitySystem::EditorVisibilitySystem()
    : mScene( Scene::Get() )
    , mEditorVisibilityModel( "editor_visibility", &RootModel::Get() )
    , mShowAllModel( VoidFunc( this, &EditorVisibilitySystem::OnShowAll ), "show_all", &mEditorVisibilityModel )
    , mSwitchCellsModel( VoidFunc( this, &EditorVisibilitySystem::OnSwitchCells ), "switch_cells", &mEditorVisibilityModel )
{
}


void EditorVisibilitySystem::Init()
{
    mOnEditorModeChanged = EventServer<map::EditorModeChangedEvent>::Get().Subscribe( boost::bind( &EditorVisibilitySystem::OnEditorModeChanged, this, _1 ) );
    mOnEditorBack = EventServer<map::EditorBackEvent>::Get().Subscribe( boost::bind( &EditorVisibilitySystem::OnEditorBack, this, _1 ) );
    mOnLayerSelected = EventServer<map::LayerSelectedEvent>::Get().Subscribe( boost::bind( &EditorVisibilitySystem::OnLayerSelected, this, _1 ) );
    mOnGroupSelected = EventServer<map::GroupSelectedEvent>::Get().Subscribe( boost::bind( &EditorVisibilitySystem::OnGroupSelected, this, _1 ) );
    mOnGroupsChanged = EventServer<map::GroupsChangedEvent>::Get().Subscribe( boost::bind( &EditorVisibilitySystem::OnGroupsChanged, this, _1 ) );
}


void EditorVisibilitySystem::Update(double DeltaTime)
{
    if (!mEnabled)
    {
        return;
    }
    static Opt<engine::KeyboardSystem> keyboard = ::engine::Engine::Get().GetSystem<engine::KeyboardSystem>();
    static Opt<engine::MouseSystem> mouse = ::engine::Engine::Get().GetSystem<MouseSystem>();
    if (keyboard->GetKey( GLFW_KEY_SPACE ).State == KeyState::Typed)
    {
        EnableSubsystems( false );
        Ui::Get().Load( "editor/visibility_hud" );
        EditorHudState::Get().SetHudShown( true );
    }
}

void EditorVisibilitySystem::OnEditorModeChanged(map::EditorModeChangedEvent const& Evt)
{
    if (Evt.mMode == "visibility")
    {
        EnableSubsystems( false );
        ::engine::Engine::Get().SetEnabled<EditorVisibilitySystem>( true );
        Ui::Get().Load( "editor/visibility_hud" );
        EditorHudState::Get().SetHudShown( true );
    }
    else
    {
        EnableSubsystems( false );
        ::engine::Engine::Get().SetEnabled<EditorVisibilitySystem>( false );

    }
}

void EditorVisibilitySystem::OnEditorBack( map::EditorBackEvent const& Evt )
{
}

void EditorVisibilitySystem::EnableSubsystems( bool enable )
{
}

Opt<EditorVisibilitySystem> EditorVisibilitySystem::Get()
{
    return engine::Engine::Get().GetSystem<EditorVisibilitySystem>();
}

void EditorVisibilitySystem::OnLayerSelected( map::LayerSelectedEvent const& Evt )
{
    if (!mEnabled)
    {
        return;
    }
    if (mInvisibleLayers.find( Evt.mLayerName ) == mInvisibleLayers.end())
    {
        mInvisibleLayers.insert( Evt.mLayerName );
    }
    else
    {
        mInvisibleLayers.erase( Evt.mLayerName );
    }
    UpdateInvisibleActors();
}



void EditorVisibilitySystem::OnGroupSelected( map::GroupSelectedEvent const& Evt )
{
    if (!mEnabled)
    {
        return;
    }
    int32_t groupId = AutoId( Evt.mGroupName );
    if (mInvisibleGroups.find( groupId ) == mInvisibleGroups.end())
    {
        mInvisibleGroups.insert( groupId );
    }
    else
    {
        mInvisibleGroups.erase( groupId );
    }
    UpdateInvisibleActors();
}

void EditorVisibilitySystem::OnShowAll()
{
    UpdateInvisibleActors();
    EditorSelectSystem::SetActorColors( mInvisibleActors, nullptr );
    mInvisibleActors.clear();
    mInvisibleGroups.clear();
    mInvisibleLayers.clear();
}

void EditorVisibilitySystem::UpdateInvisibleActors()
{
    EditorSelectSystem::SetActorColors( mInvisibleActors, nullptr );
    mInvisibleActors.clear();
    auto mapSystem = MapSystem::Get();
    ActorColors_t actorColors;
    for (auto groupId : mInvisibleGroups)
    {
        for (Opt<GroupMapElement> groupMapElement : MapElementListFilter<MapSystem::Identifier>( mapSystem->GetMapElementList(), groupId ))
        {
            for (auto targetId : groupMapElement->GetTargets())
            {
                for (auto mapElement : MapElementListFilter<MapSystem::Identifier>( mapSystem->GetMapElementList(), targetId ))
                {
                    EditorSelectSystem::AddToActorColors( mapElement->GetSpawnedActorGUID(), actorColors, &mInvisibleActors );
                }
            }
        }
    }
    auto& renderableLayer = RenderableLayer::Get();
    for (auto layerName : mInvisibleLayers)
    {
        auto layerPriority = renderableLayer( layerName );
        for (auto actor : mScene.GetActors())
        {
            auto renderableC( actor->Get<IRenderableComponent>() );
            if (renderableC.IsValid() && renderableC->GetLayerPriority() == layerPriority)
            {
                EditorSelectSystem::AddToActorColors( actor->GetGUID(), actorColors, &mInvisibleActors );
            }
        }
    }
    mInvisibleActors.insert( actorColors.begin(), actorColors.end() );
    EditorSelectSystem::SetActorColors( mInvisibleActors, &InvisibleColor );
}

void EditorVisibilitySystem::OnGroupsChanged( map::GroupsChangedEvent const& Evt )
{
    UpdateInvisibleActors();
}

void EditorVisibilitySystem::OnSwitchCells()
{
    RoomCellEditorSystem::Get()->SetCellsVisible( !RoomCellEditorSystem::Get()->IsCellsVisible() );
}

} // namespace map

