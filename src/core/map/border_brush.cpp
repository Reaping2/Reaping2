#include "border_brush.h"
#include "spawn_actor_map_element.h"
#include "map_system.h"
#include "map_element_factory.h"
#include "engine/engine.h"
#include "input/mouse.h"
#include "../position_component.h"
#include "editor_target_system.h"
#include "../i_collision_component.h"
#include "editor_grid_system.h"
#include "matrix_grid.h"
#include "neighbors.h"
#include "../border_type.h"
#include "../i_border_component.h"
#include "spawn_actor_map_element_system.h"
#include "../scene.h"
namespace map {

    BorderBrush::BorderBrush( int32_t Id )
        : IBrush( Id )
        , mMouseLeftPressed( false )
        , mMouseRightPressed( false )
    {
    }

void BorderBrush::Update( double DeltaTime )
{
    if ( !EditorTargetSystem::Get()->GetCursor().IsValid() )
    {
        return;
    }
    if ( mMouseLeftPressed && !engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Left ) )
    {
        Neighbors neighbors = EditorGridSystem::Get()->GetGrid().GetNeighbors( EditorTargetSystem::Get()->GetCursorPosition(), EditorTargetSystem::Get()->GetCursor()->GetId() );
        IBorderComponent::Borders_t borders = neighbors.GetBorders( Neighbors::GetNeighborDirs() );
        IBorderComponent::Borders_t outerBorders = neighbors.GetBorders( Neighbors::GetNeighborOuterDirs() );

        EditorTargetSystem::Get()->PutTarget( EditorTargetSystem::Get()->GetCursorPosition(), borders, outerBorders );
        Opt<engine::System> spawnActorMES( engine::Engine::Get().GetSystem<SpawnActorMapElementSystem>() );
        spawnActorMES->Update( 0 );
        mScene.InsertNewActors();
        UpdateBorders( neighbors );

        mMouseLeftPressed = false;
    }
    else if ( engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Left ) )
    {
        mMouseLeftPressed = true;
    }

    if ( mMouseRightPressed && !engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Right ) )
    {
        std::vector<int32_t> removeActors = GetActorsToRemove();

        for ( std::vector<int32_t>::iterator i = removeActors.begin(), e = removeActors.end(); i != e; ++i )
        {
            Opt<Actor> actor( mScene.GetActor( *i ) );
            Opt<IPositionComponent> positionC = actor->Get<IPositionComponent>();
            int32_t actorId = actor->GetId();
            glm::vec2 actorPos( 0.0 );
            if ( positionC.IsValid() )
            {
                actorPos = glm::vec2( positionC->GetX(), positionC->GetY() );
            }
            mScene.RemoveActor( *i );
            MapSystem::Get()->RemoveMapElement( *i );
            mScene.InsertNewActors();
            Neighbors neighbors = EditorGridSystem::Get()->GetGrid().GetNeighbors( actorPos, actorId );
            UpdateBorders( neighbors );
        }
        mMouseRightPressed = false;
    }
    else if ( engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Right ) )
    {
        mMouseRightPressed = true;
    }
}

void BorderBrush::UpdateBorders( Neighbors& neighbors )
{
    Opt<engine::System> spawnActorMES( engine::Engine::Get().GetSystem<SpawnActorMapElementSystem>() );
    std::vector<int32_t> removeActors;
    for ( Neighbors::Neighbors_t::iterator i = neighbors.mNeighbors.begin(), e = neighbors.mNeighbors.end(); i != e; ++i )
    {
        Opt<Actor> actor( Scene::Get().GetActor( i->mActorGUID ) );
        if ( !actor.IsValid() )
        {
            continue;
        }
        Opt<IPositionComponent> positionC = actor->Get<IPositionComponent>();
        if ( !positionC.IsValid() )
        {
            continue;
        }
        glm::vec2 neighborPos = glm::vec2( positionC->GetX(), positionC->GetY() );
        Neighbors neighbors2 = EditorGridSystem::Get()->GetGrid().GetNeighbors( neighborPos, actor->GetId() );
        IBorderComponent::Borders_t borders2 = neighbors2.GetBorders( Neighbors::GetNeighborDirs() );
        IBorderComponent::Borders_t outerBorders2 = neighbors2.GetBorders( Neighbors::GetNeighborOuterDirs() );
        removeActors.push_back( i->mActorGUID );
        EditorTargetSystem::Get()->PutTarget( neighborPos, borders2, outerBorders2 );
    }
    for ( std::vector<int32_t>::iterator i = removeActors.begin(), e = removeActors.end(); i != e; ++i )
    {
        mScene.RemoveActor( *i );
        MapSystem::Get()->RemoveMapElement( *i );
    }
    spawnActorMES->Update( 0 );
    mScene.InsertNewActors();
}

} // namespace map

