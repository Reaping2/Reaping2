#include "platform/i_platform.h"
#include "engine/controllers/target_player_controller_sub_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "core/i_move_component.h"
#include "core/i_position_component.h"
#include "core/target_player_controller_component.h"
#include "core/i_health_component.h"
#include "core/i_collision_component.h"

namespace engine {

TargetPlayerControllerSubSystem::TargetPlayerControllerSubSystem()
    : mScene( Scene::Get() )
{

}

void TargetPlayerControllerSubSystem::Init()
{


}

void TargetPlayerControllerSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<TargetPlayerControllerComponent> targetPCC = actor.Get<TargetPlayerControllerComponent>();
    if (!targetPCC.IsValid()||!targetPCC->IsEnabled())
    {
        return;
    }
    if (!targetPCC->GetPlayer())
    {
        return;
    }

    if (actor.Get<IMoveComponent>()->GetHeading()==0.0&&actor.Get<IMoveComponent>()->GetSpeed()==0)
    {
        actor.Get<IMoveComponent>()->SetSpeed( ((rand() % 10)+5)*20 );
    }
    Actor& player = *targetPCC->GetPlayer();
    Opt<IPositionComponent> const playerPositionC = player.Get<IPositionComponent>();
    Opt<IPositionComponent> const actorPositionC = actor.Get<IPositionComponent>();
    glm::vec2 const Diff( playerPositionC->GetX() - actorPositionC->GetX(), playerPositionC->GetY() - actorPositionC->GetY() );
    {
        BOOST_ASSERT(player.Get<ICollisionComponent>().IsValid()&&actor.Get<ICollisionComponent>().IsValid());
        double const R = player.Get<ICollisionComponent>()->GetRadius() + actor.Get<ICollisionComponent>()->GetRadius();
        if( std::abs( Diff.x ) < R && std::abs( Diff.y ) < R )
        {
            if( targetPCC->GetCounter() <= 0.0 )
            {
                Opt<IHealthComponent> healthC=player.Get<IHealthComponent>();
                if (healthC.IsValid()&&healthC->IsAlive())
                {
                    healthC->TakeDamage(1);
                }
                targetPCC->SetCounter(2.0);
            }
        }
        if( targetPCC->GetCounter() >= 0.0 )
        {
            targetPCC->SetCounter(targetPCC->GetCounter() - DeltaTime);
        }
    }
    double Rot = atan2( Diff.y, Diff.x );
    double Radians = Rot - actor.Get<IMoveComponent>()->GetHeading();
    static const double pi = boost::math::constants::pi<double>();
    while ( Radians < -pi )
    {
        Radians += pi * 2;
    }
    while ( Radians > pi )
    {
        Radians -= pi * 2;
    }
    double RotSpd = ( Radians > 0 ? 1 : -1 ) * 0.05;
    actor.Get<IMoveComponent>()->SetHeading( actor.Get<IMoveComponent>()->GetHeading() + RotSpd );
    actor.Get<IPositionComponent>()->SetOrientation( actor.Get<IMoveComponent>()->GetHeading() );
}

} // namespace engine

