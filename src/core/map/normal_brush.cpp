#include "normal_brush.h"
#include "spawn_actor_map_element.h"
#include "map_system.h"
#include "map_element_factory.h"
#include "engine/engine.h"
#include "input/mouse.h"
#include "../position_component.h"
#include "editor_target_system.h"
#include "../i_collision_component.h"
#include "editor_grid_system.h"
namespace map {

NormalBrush::NormalBrush(int32_t Id)
    : IBrush(Id)
    , mMouseLeftPressed(false)
    , mMouseRightPressed(false)
    , mScene(Scene::Get())
{

}

void NormalBrush::Update(double DeltaTime)
{
    if (mMouseLeftPressed&&!engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Left ))
    {
        EditorTargetSystem::Get()->GetTarget().PutTarget(EditorTargetSystem::Get()->GetCursorPosition());
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
            mScene.RemoveActor(*i);
            MapSystem::Get()->RemoveMapElement(*i);
        }
        mMouseRightPressed=false;
    }
    else if (engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Right ))
    {
        mMouseRightPressed=true;
    }
}

} // namespace map

