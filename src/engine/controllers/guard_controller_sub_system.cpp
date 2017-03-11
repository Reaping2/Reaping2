#include "platform/i_platform.h"
#include "engine/engine.h"
#include "engine/controllers/guard_controller_sub_system.h"
#include "core/guard_controller_component.h"
#include "core/i_target_holder_component.h"
#include "core/i_position_component.h"
#include "core/i_health_component.h"
#include "core/i_inventory_component.h"
#include "core/i_move_component.h"

namespace engine {

GuardControllerSubSystem::GuardControllerSubSystem()
    : mScene(Scene::Get())
    , mProgramState(core::ProgramState::Get())
{
}


void GuardControllerSubSystem::Init()
{
}


void GuardControllerSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<GuardControllerComponent> guardCC=actor.Get<IControllerComponent>();
    if (!guardCC.IsValid()||!guardCC->IsEnabled())
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
    guardCC->SetNextMoveTimer( guardCC->GetNextMoveTimer() - DeltaTime );
    if (currentTarget.IsValid())
    {
        auto positionC( actor.Get<IPositionComponent>() );
        auto targetPositionC( currentTarget->Get<IPositionComponent>() );
        auto const distSqr = GetDistanceSqr( positionC, targetPositionC );
        glm::vec2 const distV( (targetPositionC->GetX() - positionC->GetX()), (targetPositionC->GetY() - positionC->GetY()) );
        double const Rot = atan2( distV.y, distV.x );
        positionC->SetOrientation( Rot );
        auto inventoryC = actor.Get<IInventoryComponent>();
        if (inventoryC.IsValid())
        {
            Opt<Weapon> weapon = inventoryC->GetSelectedItem( ItemType::Weapon );
            if (weapon.IsValid())
            {
                int32_t const aggroAltDistSqr = guardCC->GetAggroAltDist() * guardCC->GetAggroAltDist();
                if (distSqr < aggroAltDistSqr)
                {
                    weapon->SetShoot( false );
                    weapon->SetShootAlt( true );
                }
                else
                {
                    weapon->SetShoot( true );
                    weapon->SetShootAlt( false );
                }
            }
        }




        if (guardCC->GetNextMoveTimer() <= 0.0)
        {
            const int32_t ran = RandomGenerator::global()() % 3;
            if (ran == 0)
            {
                guardCC->SetMoveDirection( GuardControllerComponent::Left );
            }
            else if (ran == 1)
            {
                guardCC->SetMoveDirection( GuardControllerComponent::Right );
            }
            else
            {
                guardCC->SetMoveDirection( GuardControllerComponent::None );
            }
            guardCC->SetNextMoveTimer(guardCC->GetNextMoveTimerMax() 
                + (RandomGenerator::global()() % 100*0.02*- 1)*guardCC->GetNextMoveTimerVariance() );
        }
        int32_t const tooCloseDistSqr = guardCC->GetCloseDist() * guardCC->GetCloseDist();
        int32_t const walkAwayDistSqr = guardCC->GetWalkAwayDist() * guardCC->GetWalkAwayDist();
        double heading = 0.0;
        static const double pi = boost::math::constants::pi<double>();
        if (distSqr > tooCloseDistSqr)
        {
            heading = Rot;
            if (guardCC->GetMoveDirection() == GuardControllerComponent::Left)
            {
                heading -= pi / 4;
            }
            else if (guardCC->GetMoveDirection() == GuardControllerComponent::Right)
            {
                heading += pi / 4;
            }
            moveC->SetMoving( true );
        }
        else if (distSqr < walkAwayDistSqr)
        {
            heading = Rot-pi;
            if (guardCC->GetMoveDirection() == GuardControllerComponent::Left)
            {
                heading += pi / 4;
            }
            else if (guardCC->GetMoveDirection() == GuardControllerComponent::Right)
            {
                heading -= pi / 4;
            }
            moveC->SetMoving( true );
        }
        else
        {
            heading = Rot;
            if (guardCC->GetMoveDirection() == GuardControllerComponent::Left)
            {
                heading -= pi / 2;
                moveC->SetMoving( true );
            }
            else if (guardCC->GetMoveDirection() == GuardControllerComponent::Right)
            {
                heading += pi / 2;
                moveC->SetMoving( true );
            }
            else
            {
                moveC->SetMoving( false );
            }
        }
        moveC->SetHeading( heading );
    }
    else
    {
        moveC->SetMoving( false );
        Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
        if (inventoryC.IsValid())
        {
            Opt<Weapon> weapon = inventoryC->GetSelectedItem( ItemType::Weapon );
            if (weapon.IsValid())
            {
                weapon->SetShoot( false );
                weapon->SetShootAlt( false );
            }
        }
    }
}


void GuardControllerSubSystem::UpdateTarget( Actor& actor, Opt<ITargetHolderComponent> targetHolderC )
{
    Opt<Actor> currentTarget( mScene.GetActor( targetHolderC->GetTargetGUID() ) );
    Opt<GuardControllerComponent> guardCC = actor.Get<IControllerComponent>();
    auto positionC( actor.Get<IPositionComponent>() );
    if (currentTarget.IsValid())
    {
        auto targetPositionC( currentTarget->Get<IPositionComponent>() );
        int32_t const disaggroDistSqr = guardCC->GetPeaceDist()*guardCC->GetPeaceDist();
        if (GetDistanceSqr( positionC, targetPositionC) > disaggroDistSqr )
        {
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
            int32_t const aggroDistSqr = guardCC->GetAggroDist() * guardCC->GetAggroDist();
            if (GetDistanceSqr( positionC, targetPositionC ) < aggroDistSqr )
            {
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

int32_t GuardControllerSubSystem::GetDistanceSqr( Opt<IPositionComponent> positionC, Opt<IPositionComponent> otherPositionC )
{
    glm::vec2 const distV( (positionC->GetX() - otherPositionC->GetX()), (positionC->GetY() - otherPositionC->GetY()) );
    return glm::dot( distV, distV );
}

} // namespace engine

