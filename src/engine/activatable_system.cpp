#include "platform/i_platform.h"
#include "activatable_system.h"
#include "core/i_activatable_component.h"
#include "core/i_health_component.h"
#include "core/i_position_component.h"
#include "engine.h"
#include "collision_system.h"
#include "core/i_controller_component.h"
#include "core/player_controller_component.h"

namespace engine {

ActivatableSystem::ActivatableSystem()
{
}


const int32_t ActivatableSystem::ActivatableMask = 1 << CollisionClass::Pickup | 1 << CollisionClass::Flag | 1 << CollisionClass::Wall;

void ActivatableSystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IActivatableComponent>().IsValid(); } );
}


void ActivatableSystem::Update(double DeltaTime)
{
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
        Opt<IActivatableComponent> activatableC=actor->Get<IActivatableComponent>();
        if (!activatableC.IsValid())
        {
            continue;
        }
        activatableC->SetHighlighted( false );
        activatableC->SetActivatorGUID( -1 );
    }
    for (auto& clientData : mProgramState.mClientDatas)
    {
        auto player( mScene.GetActor( clientData.mClientActorGUID ) );
        if (!player.IsValid())
        {
            continue;
        }
        auto healthC( player->Get<IHealthComponent>() );
        if (healthC.IsValid() && !healthC.IsValid())
        {
            continue;
        }
        auto positionC( player->Get<IPositionComponent>() );
        if (!positionC.IsValid())
        {
            continue;
        }
        Opt<PlayerControllerComponent> playerCC( player->Get<IControllerComponent>() );
        if (!playerCC.IsValid())
        {
            continue;
        }

        playerCC->mHighLightedGUID = -1;

        static auto collisionSystem( ::engine::Engine::Get().GetSystem<engine::CollisionSystem>() );
        auto&& activatables = collisionSystem->GetAllCollidingActors(
            glm::vec2( positionC->GetX(), positionC->GetY() ),
            playerCC->mActivateRange,
            ActivatableMask );

        // TODO: i am almost certain that simply the closest actor is a good choice. need to test the augmented highlight
        double const absOrientation = std::abs( positionC->GetOrientation() );
        double minDiff = std::numeric_limits<double>::max();
        Opt<Actor> minTarget;
        glm::vec2 minDistV;
        for (auto&& target : activatables)
        {
            auto targetActivatableC( target->Get<IActivatableComponent>() );
            if (!targetActivatableC.IsValid()||!targetActivatableC->IsEnabled())
            {
                continue;
            }
            auto targetPositionC( target->Get<IPositionComponent>() );
            if (!targetPositionC.IsValid())
            {
                continue;
            }
            glm::vec2 const distV( (targetPositionC->GetX() - positionC->GetX()), (targetPositionC->GetY() - positionC->GetY()) );
            double absDirection = absOrientation;
            if (distV.y != 0.0&&distV.x != 0)
            {
                absDirection = std::abs( atan2( distV.y, distV.x ) );
            }
            double const absDiff = std::abs( absDirection - absOrientation );
            if (absDiff < minDiff)
            {
                minDiff = absDiff;
                minTarget = target;
                minDistV = distV;
            }
        }
        if (minTarget.IsValid())
        {
            auto closerTarget =
                collisionSystem->GetFirstCollidingActor(
                    *player, minDistV,
                    15.0,
                    ActivatableMask,
                    []( Actor const& actor )->bool
                    { 
                        Opt<IActivatableComponent> const activatableC( actor.Get<IActivatableComponent>() );
                        return activatableC.IsValid() && activatableC->IsEnabled();
                    } );
            if (closerTarget.IsValid())
            {
                auto closerActivatableC( closerTarget->Get<IActivatableComponent>() );
                if (closerActivatableC.IsValid() && closerActivatableC->IsEnabled())
                {
                    minTarget = closerTarget;
                }
            }
            auto targetActivatableC(minTarget->Get<IActivatableComponent>());
            if (targetActivatableC.IsValid())
            {
                targetActivatableC->SetHighlighted( true );
                playerCC->mHighLightedGUID = minTarget->GetGUID();
                if (playerCC->mActivate.GetValue())
                {
                    targetActivatableC->SetActivatorGUID( player->GetGUID() );
                    playerCC->mActivate.Handled();
                }
            }
        }
    }
    for (auto& clientData : mProgramState.mClientDatas)
    {
        auto player( mScene.GetActor( clientData.mClientActorGUID ) );
        if (!player.IsValid())
        {
            continue;
        }
        Opt<PlayerControllerComponent> pcc( player->Get<IControllerComponent>() );
        if (pcc.IsValid() && pcc->mActivate.GetValue())
        {
            pcc->mActivate.Handled();
        }
    }
}


} // namespace engine

