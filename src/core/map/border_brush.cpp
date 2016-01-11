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

BorderBrush::BorderBrush(int32_t Id)
    : IBrush(Id)
    , mMouseLeftPressed(false)
    , mMouseRightPressed(false)
    , mScene(Scene::Get())
{
    Neighbors::NeighborMap_t neighborMap;
    neighborMap[BorderType::Top]=Neighbors::Other;
    mNeighborDirMap.push_back(neighborMap);neighborMap.clear();    //top

    neighborMap[BorderType::Top]=Neighbors::Same;
    neighborMap[BorderType::TopRight]=Neighbors::Other;
    neighborMap[BorderType::Right]=Neighbors::Same;
    mNeighborDirMap.push_back(neighborMap);neighborMap.clear();    //topright

    neighborMap[BorderType::Right]=Neighbors::Other;
    mNeighborDirMap.push_back(neighborMap);neighborMap.clear();    //right

    neighborMap[BorderType::Right]=Neighbors::Same;
    neighborMap[BorderType::BottomRight]=Neighbors::Other;
    neighborMap[BorderType::Bottom]=Neighbors::Same;
    mNeighborDirMap.push_back(neighborMap);neighborMap.clear();    //bottomright

    neighborMap[BorderType::Bottom]=Neighbors::Other;
    mNeighborDirMap.push_back(neighborMap);neighborMap.clear();    //bottom

    neighborMap[BorderType::Left]=Neighbors::Same;
    neighborMap[BorderType::BottomLeft]=Neighbors::Other;
    neighborMap[BorderType::Bottom]=Neighbors::Same;
    mNeighborDirMap.push_back(neighborMap);neighborMap.clear();    //bottomleft

    neighborMap[BorderType::Left]=Neighbors::Other;
    mNeighborDirMap.push_back(neighborMap);neighborMap.clear();    //left

    neighborMap[BorderType::Top]=Neighbors::Same;
    neighborMap[BorderType::TopLeft]=Neighbors::Other;
    neighborMap[BorderType::Left]=Neighbors::Same;
    mNeighborDirMap.push_back(neighborMap);neighborMap.clear();    //topleft
}

void BorderBrush::Update(double DeltaTime)
{
    if (mMouseLeftPressed&&!engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Left ))
    {
        Neighbors neighbors=EditorGridSystem::Get()->GetGrid().GetNeighbors(EditorTargetSystem::Get()->GetCursorPosition(),EditorTargetSystem::Get()->GetCursor()->GetId());
        IBorderComponent::Borders_t borders=GetBorders(neighbors);

        EditorTargetSystem::Get()->GetTarget().PutTarget(EditorTargetSystem::Get()->GetCursorPosition(),borders);
        Opt<engine::System> spawnActorMES(engine::Engine::Get().GetSystem<SpawnActorMapElementSystem>());
        spawnActorMES->Update(0);
        mScene.Update(0);
        UpdateBorders(neighbors);

        mMouseLeftPressed=false;
    }
    else if (engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Left ))
    {
        mMouseLeftPressed=true;
    }
    
    if (mMouseRightPressed&&!engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Right ))
    {
        std::vector<int32_t> removeActors;
        glm::vec2 mousePos=EditorGridSystem::Get()->GetGrid().GetMousePosition();
        int32_t curGUID=EditorTargetSystem::Get()->GetCursor()->GetGUID();
        for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
        {
            Actor& actor=**it;
            Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
            Opt<ICollisionComponent> collisionC = actor.Get<ICollisionComponent>();
            if (!positionC.IsValid()||!collisionC.IsValid())
            {
                continue;
            }
            if (curGUID!=actor.GetGUID()
                &&std::abs(positionC->GetX()-mousePos.x)<collisionC->GetRadius()
                &&std::abs(positionC->GetY()-mousePos.y)<collisionC->GetRadius())
            {
                removeActors.push_back(actor.GetGUID());
            }
        }
        for (std::vector<int32_t>::iterator i=removeActors.begin(), e=removeActors.end();i!=e;++i)
        {
            Opt<Actor> actor(mScene.GetActor(*i));
            Opt<IPositionComponent> positionC = actor->Get<IPositionComponent>();
            int32_t actorId=actor->GetId();
            glm::vec2 actorPos(0.0);
            if (positionC.IsValid())
            {
                actorPos=glm::vec2(positionC->GetX(),positionC->GetY());
            }
            mScene.RemoveActor(*i);
            MapSystem::Get()->RemoveMapElement(*i);
            mScene.Update(0);
            Neighbors neighbors=EditorGridSystem::Get()->GetGrid().GetNeighbors(actorPos,actorId);
            UpdateBorders(neighbors);
        }
        mMouseRightPressed=false;
    }
    else if (engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Right ))
    {
        mMouseRightPressed=true;
    }
}

IBorderComponent::Borders_t BorderBrush::GetBorders(Neighbors &neighbors)
{
    IBorderComponent::Borders_t r;
    for (size_t i=0;i<BorderType::Num_Classes;++i)
    {
        if (neighbors.IsMatching(mNeighborDirMap[i]))
        {
            r.push_back(static_cast<BorderType::Type>(i));
        }
    }
    return r;
}

void BorderBrush::UpdateBorders(Neighbors &neighbors)
{
    Opt<engine::System> spawnActorMES(engine::Engine::Get().GetSystem<SpawnActorMapElementSystem>());
    std::vector<int32_t> removeActors;
    for (Neighbors::Neighbors_t::iterator i=neighbors.mNeighbors.begin(),e=neighbors.mNeighbors.end();i!=e;++i)
    {
        Opt<Actor> actor(Scene::Get().GetActor(i->mActorGUID));
        if (!actor.IsValid())
        {
            continue;
        }
        Opt<IPositionComponent> positionC=actor->Get<IPositionComponent>();
        if (!positionC.IsValid())
        {
            continue;
        }
        glm::vec2 neighborPos=glm::vec2(positionC->GetX(),positionC->GetY());
        Neighbors neighbors2=EditorGridSystem::Get()->GetGrid().GetNeighbors(neighborPos,actor->GetId());
        IBorderComponent::Borders_t borders2=GetBorders(neighbors2);
        removeActors.push_back(i->mActorGUID);
        EditorTargetSystem::Get()->GetTarget().PutTarget(neighborPos,borders2);
    }
    for (std::vector<int32_t>::iterator i=removeActors.begin(), e=removeActors.end();i!=e;++i)
    {
        mScene.RemoveActor(*i);
        MapSystem::Get()->RemoveMapElement(*i);
    }
    spawnActorMES->Update(0);
    mScene.Update(0);
}

} // namespace map

