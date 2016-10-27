#include "platform/i_platform.h"
#include "engine/engine.h"
#include "engine/controllers/path_controller_sub_system.h"
#include "core/path_controller_component.h"
#include "core/i_position_component.h"
#include "core/i_health_component.h"
#include "core/i_move_component.h"
#include "core/i_collision_component.h"

namespace engine {

PathControllerSubSystem::PathControllerSubSystem()
    : mScene(Scene::Get())
    , mProgramState(core::ProgramState::Get())
{
}


void PathControllerSubSystem::Init()
{
    mPathSystem = engine::Engine::Get().GetSystem<engine::path::PathSystem>();
}


void PathControllerSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<PathControllerComponent> pathCC=actor.Get<IControllerComponent>();
    if (!pathCC.IsValid()||!pathCC->IsEnabled())
    {
        return;
    }
    if (mProgramState.mMode == core::ProgramState::Client)
    {
        return;
    }
    auto targetHolderC = actor.Get<ITargetHolderComponent>();
    if (!targetHolderC.IsValid())
    {
        return;
    }
    auto healthC = actor.Get<IHealthComponent>();
    if (!healthC.IsValid() || !healthC->IsAlive())
    {
        return;
    }
    UpdateTarget( actor, targetHolderC );
    Opt<Actor> currentTarget( mScene.GetActor( targetHolderC->GetTargetGUID() ) );
    auto moveC( actor.Get<IMoveComponent>() );
    auto positionC( actor.Get<IPositionComponent>() );
    if (currentTarget.IsValid())
    {
        if (mPathSystem->GetBoxIndex( actor ) == mPathSystem->GetBoxIndex( *currentTarget ))
        {
            auto targetPositionC( currentTarget->Get<IPositionComponent>() );
            glm::vec2 const distV( (targetPositionC->GetX() - positionC->GetX()), (targetPositionC->GetY() - positionC->GetY()) );
            moveC->SetHeading( atan2( distV.y, distV.x ) );
        }
        else
        {
            moveC->SetHeading( mPathSystem->GetDirection( actor, *currentTarget ) );
        }
        positionC->SetOrientation( moveC->GetHeading() );
        moveC->SetMoving( true );
        Opt<IPositionComponent> const targetPositionC = currentTarget->Get<IPositionComponent>();
        glm::vec2 const Diff( targetPositionC->GetX() - positionC->GetX(), targetPositionC->GetY() - positionC->GetY() );
        double const R = currentTarget->Get<ICollisionComponent>()->GetRadius() + actor.Get<ICollisionComponent>()->GetRadius();
        if (std::abs( Diff.x ) < R && std::abs( Diff.y ) < R)
        {
            if (pathCC->GetNextAttackTimer() <= 0.0)
            {
                Opt<IHealthComponent> healthC = currentTarget->Get<IHealthComponent>();
                if (healthC.IsValid() && healthC->IsAlive())
                {
                    healthC->TakeDamage( pathCC->GetDamage() );
                }
                pathCC->SetNextAttackTimer( pathCC->GetNextAttackTimerMax() );
            }
        }
    }
    else
    {
        moveC->SetMoving( false );
    }

    if (pathCC->GetNextAttackTimer() > 0.0)
    {
        pathCC->SetNextAttackTimer( pathCC->GetNextAttackTimer() - DeltaTime );
    }

}


void PathControllerSubSystem::UpdateTarget( Actor& actor, Opt<ITargetHolderComponent> targetHolderC )
{
    Opt<Actor> currentTarget( mScene.GetActor( targetHolderC->GetTargetGUID() ) );
    Opt<PathControllerComponent> pathCC = actor.Get<IControllerComponent>();
    auto positionC( actor.Get<IPositionComponent>() );
    if (currentTarget.IsValid())
    {
        auto targetPositionC( currentTarget->Get<IPositionComponent>() );
        double const distance = mPathSystem->GetDistance( actor, *currentTarget );
        L2( "Distance to target: %f\n", distance );
        if (distance > pathCC->GetPeaceDist())
        {
            L2( "Peace from target: %f\n", distance );
            targetHolderC->SetTargetGUID( -1 );
            currentTarget.Reset();
        }
    }
    if (!currentTarget.IsValid())
    {
        if (!positionC.IsValid())
        {
            return;
        }
        for (auto player : ActorListFilter<Scene::CollisionClassActors>( mScene.GetActors(), CollisionClass::Player ))
        {
            auto targetPositionC( player->Get<IPositionComponent>() );
            if (!targetPositionC.IsValid())
            {
                return;
            }
            double const distance = mPathSystem->GetDistance( actor, *player );
            if ( distance < pathCC->GetAggroDist())
            {
                L2( "Aggro to target: %f\n", distance );
                targetHolderC->SetTargetGUID( player->GetGUID() );
                currentTarget = mScene.GetActor( targetHolderC->GetTargetGUID() );
                break;
            }
        }
    }
    if (currentTarget.IsValid())
    {
        Opt<IHealthComponent> healthC = currentTarget->Get<IHealthComponent>();
        if (!healthC.IsValid() || !healthC->IsAlive())
        {
            targetHolderC->SetTargetGUID( -1 );
            currentTarget.Reset();
        }
    }
}

} // namespace engine

