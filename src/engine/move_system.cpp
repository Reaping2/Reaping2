#include "platform/i_platform.h"
#include "engine/engine.h"
#include "engine/move_system.h"
#include "engine/activity_system.h"
#include "core/i_move_component.h"
#include "core/i_position_component.h"
#include "core/i_collision_component.h"

namespace engine {

MoveSystem::MoveSystem()
    : mScene( Scene::Get() )
{

}

void MoveSystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IMoveComponent>().IsValid()
            && actor.Get<IPositionComponent>().IsValid(); } );
}

void MoveSystem::Update( double DeltaTime )
{
    static auto activityS = engine::Engine::Get().GetSystem<ActivitySystem>();
    for (auto actor : activityS->GetActiveActors() )
    {
        Opt<IMoveComponent> moveC = actor->Get<IMoveComponent>();
        if ( !moveC.IsValid() )
        {
            continue;
        }
        Opt<IPositionComponent> positionC = actor->Get<IPositionComponent>();
        if ( !positionC.IsValid() )
        {
            continue;
        }
        moveC->SetHeading( moveC->GetHeading() + DeltaTime * moveC->GetHeadingModifier() );
        if ( moveC->IsMoving() )
        {
            positionC->SetX( positionC->GetX() + DeltaTime * moveC->GetSpeedX() );
            positionC->SetY( positionC->GetY() + DeltaTime * moveC->GetSpeedY() );
        }
    }

}

} // namespace engine

