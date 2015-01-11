#include "platform/i_platform.h"
#include "engine/controllers/target_player_controller_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "core/i_move_component.h"
#include "core/i_position_component.h"
#include "core/target_player_controller_component.h"
#include "core/damage_action.h"

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
        actor.AddAction( AutoId( "move" ) ); //TODO: there will be no actions, this one wont be needed
        actor.Get<IMoveComponent>()->SetSpeed( ((rand() % 10)+5)*0.02 );
    }
    Actor& player = *targetPCC->GetPlayer();
    Opt<IPositionComponent> const playerPositionC = player.Get<IPositionComponent>();
    Opt<IPositionComponent> const actorPositionC = actor.Get<IPositionComponent>();
    glm::vec2 const Diff( playerPositionC->GetX() - actorPositionC->GetX(), playerPositionC->GetY() - actorPositionC->GetY() );
    {
        // todo: biteaction
        BOOST_ASSERT(player.Get<ICollisionComponent>().IsValid()&&actor.Get<ICollisionComponent>().IsValid());
        double const R = player.Get<ICollisionComponent>()->GetRadius() + actor.Get<ICollisionComponent>()->GetRadius();
        if( std::abs( Diff.x ) < R && std::abs( Diff.y ) < R )
        {
            if( targetPCC->GetCounter() <= 0.0 )
            {
                std::auto_ptr<Action> act = ActionRepo::Get()( AutoId("damage") );
                DamageAction* dact = static_cast<DamageAction*>(act.release());
                dact->SetDamage( 1 );
                player.AddAction(std::auto_ptr<Action>(static_cast<Action*>(dact)));
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

