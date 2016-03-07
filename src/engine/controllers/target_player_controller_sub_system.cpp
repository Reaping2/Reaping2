#include "platform/i_platform.h"
#include "engine/controllers/target_player_controller_sub_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "core/i_move_component.h"
#include "core/i_position_component.h"
#include "core/target_player_controller_component.h"
#include "core/i_health_component.h"
#include "core/i_collision_component.h"
#include "platform/event.h"
#include "core/i_target_holder_component.h"

namespace engine {

TargetPlayerControllerSubSystem::TargetPlayerControllerSubSystem()
    : mScene( Scene::Get() )
    , mProgramState ( core::ProgramState::Get() )
{

}

void TargetPlayerControllerSubSystem::Init()
{


}

void TargetPlayerControllerSubSystem::Update( Actor& actor, double DeltaTime )
{
    Opt<TargetPlayerControllerComponent> targetPCC = actor.Get<TargetPlayerControllerComponent>();
    if ( !targetPCC.IsValid() || !targetPCC->IsEnabled() )
    {
        return;
    }
    Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
    if ( healthC.IsValid() && !healthC->IsAlive() )
    {
        return;
    }

    actor.Get<IPositionComponent>()->SetOrientation( actor.Get<IMoveComponent>()->GetHeading() );

    Opt<ITargetHolderComponent> targetHolderC = actor.Get<ITargetHolderComponent>();
    if ( !targetHolderC.IsValid() )
    {
        return;
    }
    if ( mProgramState.mMode == core::ProgramState::Client )
    {
        return;
    }

    UpdateTarget( targetHolderC );
    Opt<Actor> currentTarget( mScene.GetActor( targetHolderC->GetTargetGUID() ) );

    if( !currentTarget.IsValid() )
    {
        return;
    }

    if ( targetPCC->IsNeedInit() )
    {
        Opt<IMoveComponent> moveC = actor.Get<IMoveComponent>();
        moveC->GetSpeed().mBase.Set( ( ( rand() % 10 ) + 5 ) * 20 );
        moveC->SetMoving( true );
        targetPCC->SetNeedInit( false );
    }


    double Radians = TargetPlayerControllerSubSystem::GetRotationDiffRadians( actor, *currentTarget );


    if( targetPCC->GetHeadingModifierCounter() > 0.0 )
    {
        targetPCC->SetHeadingModifierCounter( targetPCC->GetHeadingModifierCounter() - DeltaTime );
    }
    if( targetPCC->GetHeadingModifierCounter() <= 0.0 )
    {
        targetPCC->SetHeadingModifierCounter( targetPCC->GetHeadingModifierFrequency() );
        actor.Get<IMoveComponent>()->SetHeadingModifier( ( Radians > 0 ? 1 : -1 ) * 1.5 );
    }

    if( std::abs( Radians ) < 0.1 )
    {
        actor.Get<IMoveComponent>()->SetHeadingModifier( 0.0 );
    }

    {
        Opt<IPositionComponent> const playerPositionC = currentTarget->Get<IPositionComponent>();
        Opt<IPositionComponent> const actorPositionC = actor.Get<IPositionComponent>();
        glm::vec2 const Diff( playerPositionC->GetX() - actorPositionC->GetX(), playerPositionC->GetY() - actorPositionC->GetY() );
        BOOST_ASSERT( currentTarget->Get<ICollisionComponent>().IsValid() && actor.Get<ICollisionComponent>().IsValid() );
        double const R = currentTarget->Get<ICollisionComponent>()->GetRadius() + actor.Get<ICollisionComponent>()->GetRadius();
        if( std::abs( Diff.x ) < R && std::abs( Diff.y ) < R )
        {
            if( targetPCC->GetAttackCounter() <= 0.0 )
            {
                Opt<IHealthComponent> healthC = currentTarget->Get<IHealthComponent>();
                if ( healthC.IsValid() && healthC->IsAlive() )
                {
                    healthC->TakeDamage( 4 );
                }
                targetPCC->SetAttackCounter( targetPCC->GetAttackFrequency() );
            }
        }
        if( targetPCC->GetAttackCounter() > 0.0 )
        {
            targetPCC->SetAttackCounter( targetPCC->GetAttackCounter() - DeltaTime );
        }
    }
}

void TargetPlayerControllerSubSystem::UpdateTarget( Opt<ITargetHolderComponent> targetHolderC )
{
    Opt<Actor> currentTarget( mScene.GetActor( targetHolderC->GetTargetGUID() ) );

    if ( !currentTarget.IsValid() )
    {
        ActorListFilter<Scene::CollisionClassActors> wrp( mScene.GetActors(), CollisionClass::Player ); //=Object.GetActors<Scene::RenderableComponents>();

        if( wrp.size() != 0 )
        {
            size_t selectedTarget = rand() % wrp.size();
            ActorListFilter<Scene::CollisionClassActors>::const_iterator i = wrp.begin(), e = wrp.end();
            for( size_t c = 0; i != e && c < selectedTarget; ++i, ++c )
            {}

            if( i != e )
            {
                targetHolderC->SetTargetGUID( ( *i )->GetGUID() );
            }
        }
    }
    currentTarget = mScene.GetActor( targetHolderC->GetTargetGUID() );
    if( currentTarget.IsValid() )
    {
        Opt<IHealthComponent> healthC = currentTarget->Get<IHealthComponent>();
        if( !healthC.IsValid() || !healthC->IsAlive() )
        {
            targetHolderC->SetTargetGUID( -1 );
        }
    }
}
// return value usage: direction:( Radians > 0 ? 1 : -1 ) * 1;
double TargetPlayerControllerSubSystem::GetRotationDiffRadians( Actor const& actor, Actor const& target )
{
    Opt<IPositionComponent> const playerPositionC = target.Get<IPositionComponent>();
    Opt<IPositionComponent> const actorPositionC = actor.Get<IPositionComponent>();
    glm::vec2 const Diff( playerPositionC->GetX() - actorPositionC->GetX(), playerPositionC->GetY() - actorPositionC->GetY() );
    static const double pi = boost::math::constants::pi<double>();
    double Radians = 2 * pi;
    double Rot = atan2( Diff.y, Diff.x );
    Radians = Rot - actor.Get<IMoveComponent>()->GetHeading();
    while ( Radians < -pi )
    {
        Radians += pi * 2;
    }
    while ( Radians > pi )
    {
        Radians -= pi * 2;
    }
    return Radians; // direction:( Radians > 0 ? 1 : -1 ) * 1;
}

} // namespace engine

