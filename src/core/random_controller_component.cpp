#include "platform/i_platform.h"
#include "core/random_controller_component.h"
#include "core/i_move_component.h"
#include "core/i_health_component.h"
#include "core/i_position_component.h"

RandomControllerComponent::RandomControllerComponent()
    : ControllerComponent()
    , mCounter( 2 )
    , mHeadingModifier( 0 )
{

}

void RandomControllerComponent::SetActor( Actor* Obj )
{
    IControllerComponent::SetActor(Obj);
    if( !mActor )
    {
        return;
    }
    mActor->Get<IMoveComponent>()->SetHeading( rand() % 180 );
    mActor->AddAction( AutoId( "move" ) );
}

void RandomControllerComponent::Update( double Seconds )
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


void RandomControllerComponentLoader::BindValues()
{

}

RandomControllerComponentLoader::RandomControllerComponentLoader()
{

}
