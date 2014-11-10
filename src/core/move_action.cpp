#include "i_core.h"

MoveAction::MoveAction( int32_t Id )
    : Action( Id )
	, ComponentDependent()
{
    mAreBlockedActionsExcluded = false;
    mCancelledActionIds.push_back( IdStorage::Get().GetId( "idle" ) );
    mSecsToEnd = 1;
    mIsLoop = true;
	AddDependency( AutoId("position_component") );
}
void MoveAction::SetActor( Actor* Obj )
{
	//TODO: generally this one will move to Action, In the end no actor will be set. Actor is a Component
	//Components have components. Will be checked by InitDependencies, and DependentComponents will be taken automatically
	//if the component fails to fulfil DependentComponents, it wont trigger the action.
	Action::SetActor( Obj );
	InitDependencies( *Obj );
}

void MoveAction::Update( double Seconds )
{
    if( !mActor )
    {
        return;
    }
    Action::Update( Seconds );
	PositionComponent& positionC = GetDependentComponent<PositionComponent>( AutoId("position_component") );
	PositionComponent& actorPositionC = mActor->GetComponent<PositionComponent>( AutoId("position_component") );
	positionC.SetX( actorPositionC.GetX() + Seconds * mActor->GetSpeedX() );
	positionC.SetY( actorPositionC.GetY() + Seconds * mActor->GetSpeedY() );

    mActor->ClipScene();
}