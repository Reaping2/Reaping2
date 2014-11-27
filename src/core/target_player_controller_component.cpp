#include "i_core.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/i_health_component.h"
#include "core/target_player_controller_component.h"
#include "core/damage_action.h"
#include "core/i_collision_component.h"

TargetPlayerControllerComponent::TargetPlayerControllerComponent()
    : mPlayer( NULL )
    , mCounter( 0.0 )
    , mHeadingModifier( 0 )
{

}

void TargetPlayerControllerComponent::SetPlayer( Actor* Player )
{
    mPlayer = Player;
}

void TargetPlayerControllerComponent::SetActor( Actor* Obj )
{
    IControllerComponent::SetActor(Obj);
    if( !mActor )
    {
        return;
    }
    mActor->Get<IMoveComponent>()->SetHeading( 0 );
    mActor->Get<IMoveComponent>()->SetSpeed( 0.1 );
    mActor->AddAction( AutoId( "move" ) );
}

void TargetPlayerControllerComponent::Update( double Seconds )
{
    if( !mActor )
    {
        return;
    }
    if( !mPlayer )
    {
        return;
    }
    if ( !mActor->Get<IHealthComponent>()->IsAlive() )
    {
        return;
    }
    Opt<IPositionComponent> const playerPositionC = mPlayer->Get<IPositionComponent>();
    Opt<IPositionComponent> const actorPositionC = mActor->Get<IPositionComponent>();
    glm::vec2 const Diff( playerPositionC->GetX() - actorPositionC->GetX(), playerPositionC->GetY() - actorPositionC->GetY() );
    {
        // todo: biteaction
        BOOST_ASSERT(mPlayer->Get<ICollisionComponent>().IsValid()&&mActor->Get<ICollisionComponent>().IsValid());
        double const R = mPlayer->Get<ICollisionComponent>()->GetRadius() + mActor->Get<ICollisionComponent>()->GetRadius();
        if( std::abs( Diff.x ) < R && std::abs( Diff.y ) < R )
        {
            if( mCounter <= 0.0 )
            {
                std::auto_ptr<Action> act = ActionRepo::Get()( AutoId("damage") );
                DamageAction* dact = static_cast<DamageAction*>(act.release());
                dact->SetDamage( 1 );
                mPlayer->AddAction(std::auto_ptr<Action>(static_cast<Action*>(dact)));
                mCounter = 2.0;
            }
        }
        if( mCounter >= 0.0 )
        {
            mCounter -= Seconds;
        }
    }
    double Rot = atan2( Diff.y, Diff.x );
    double Radians = Rot - mActor->Get<IMoveComponent>()->GetHeading();
    static const double pi = boost::math::constants::pi<double>();
    while ( Radians < -pi )
    {
        Radians += pi * 2;
    }
    while ( Radians > pi )
    {
        Radians -= pi * 2;
    }
    double RotSpd = ( Radians > 0 ? 1 : -1 ) * 0.01;
    mActor->Get<IMoveComponent>()->SetHeading( mActor->Get<IMoveComponent>()->GetHeading() + RotSpd );
    mActor->Get<IPositionComponent>()->SetOrientation( mActor->Get<IMoveComponent>()->GetHeading() );
}
