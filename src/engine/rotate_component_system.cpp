#include "platform/i_platform.h"
#include "rotate_component_system.h"
#include "core/i_rotate_component.h"
#include "core/i_position_component.h"
#include "system_suppressor.h"

namespace engine {

RotateComponentSystem::RotateComponentSystem()
    : mScene( Scene::Get() )
{
}


void RotateComponentSystem::Init()
{
    SystemSuppressor::Get().Add( SystemSuppressor::SceneLoad, GetType_static() );

    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IRotateComponent>().IsValid()
            && actor.Get<IPositionComponent>().IsValid(); } );
}


void RotateComponentSystem::Update(double DeltaTime)
{
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
       Opt<IRotateComponent> rotateC=actor->Get<IRotateComponent>();
       if (!rotateC.IsValid()||!rotateC->IsRotating())
       {
           continue;
       }
       Opt<IPositionComponent> positionC = actor->Get<IPositionComponent>();
       if (!positionC.IsValid())
       {
           continue;
       }
       positionC->SetOrientation( positionC->GetOrientation() + rotateC->GetSpeed()*DeltaTime );
    }
}


} // namespace engine

