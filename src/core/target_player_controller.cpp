#include "i_core.h"
#include "core/i_position_component.h"

TargetPlayerController::TargetPlayerController( Actor* player ): Controller()
    , mCounter( 0.0 )
    , mHeadingModifier( 0 )
    , mPlayer( player )
{

}

void TargetPlayerController::SetActor( Actor* Obj )
{
    Controller::SetActor( Obj );
    if( !mActor )
    {
        return;
    }
    mActor->SetHeading( 0 );
    mActor->SetSpeed( 0.1 );
    mActor->AddAction( AutoId( "move" ) );
}

void TargetPlayerController::Update( double Seconds )
{
    if( !mActor )
    {
        return;
    }
    if( !mPlayer )
    {
        return;
    }
    Opt<IPositionComponent> const playerPositionC = mPlayer->Get<IPositionComponent>();
    Opt<IPositionComponent> const actorPositionC = mActor->Get<IPositionComponent>();
    glm::vec2 const Diff( playerPositionC->GetX() - actorPositionC->GetX(), playerPositionC->GetY() - actorPositionC->GetY() );
    {
        // todo: biteaction
        double const R = mPlayer->GetRadius() + mActor->GetRadius();
        if( std::abs( Diff.x ) < R && std::abs( Diff.y ) < R )
        {
            if( mCounter <= 0.0 )
            {
                mPlayer->TakeDamage( 1 );
                mCounter = 2.0;
            }
        }
        if( mCounter >= 0.0 )
        {
            mCounter -= Seconds;
        }
    }
    double Rot = atan2( Diff.y, Diff.x );
    double Radians = Rot - mActor->GetHeading();
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
    mActor->SetHeading( mActor->GetHeading() + RotSpd );
    mActor->Get<IPositionComponent>()->SetOrientation( mActor->GetHeading() );
}
