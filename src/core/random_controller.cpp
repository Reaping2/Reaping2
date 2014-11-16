#include "i_core.h"
#include "core/i_move_component.h"

RandomController::RandomController(): Controller()
    , mCounter( 2 )
    , mHeadingModifier( 0 )
{

}

void RandomController::SetActor( Actor* Obj )
{
    Controller::SetActor( Obj );
    if( !mActor )
    {
        return;
    }
    mActor->Get<IMoveComponent>()->SetHeading( rand() % 180 );
    mActor->AddAction( AutoId( "move" ) );
}

void RandomController::Update( double Seconds )
{
    if( !mActor )
    {
        return;
    }
    mCounter += Seconds;
    if ( mCounter > 2 )
    {
        mCounter = 0;
        mHeadingModifier = ( rand() % 10 - 5 ) * .33;

        mActor->Get<IMoveComponent>()->SetSpeed( ( rand() % 10 )*.03 );
    }
    mActor->Get<IMoveComponent>()->SetHeading( mActor->Get<IMoveComponent>()->GetHeading() + Seconds * mHeadingModifier );
    mActor->Get<IPositionComponent>()->SetOrientation( mActor->Get<IMoveComponent>()->GetHeading() );
}

