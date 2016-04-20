#include "platform/i_platform.h"
#include "rotate_component_system.h"
#include "core/i_rotate_component.h"
#include "../core/i_position_component.h"

namespace engine {

RotateComponentSystem::RotateComponentSystem()
    : mScene( Scene::Get() )
{
}


void RotateComponentSystem::Init()
{
}


void RotateComponentSystem::Update(double DeltaTime)
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
       Actor& actor = **it;
       Opt<IRotateComponent> rotateC=actor.Get<IRotateComponent>();
       if (!rotateC.IsValid()||!rotateC->IsRotating())
       {
           continue;
       }
       Opt<IPositionComponent> positionC = actor.Get<IPositionComponent>();
       if (!positionC.IsValid())
       {
           continue;
       }
       positionC->SetOrientation( positionC->GetOrientation() + rotateC->GetSpeed()*DeltaTime );
    }
}


} // namespace engine

