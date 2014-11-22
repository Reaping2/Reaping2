#include "i_core.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/i_collision_component.h"

MoveAction::MoveAction( int32_t Id )
    : Action( Id )
{
    mAreBlockedActionsExcluded = false;
    mCancelledActionIds.push_back( IdStorage::Get().GetId( "idle" ) );
    mSecsToEnd = 1;
    mIsLoop = true;
}
void MoveAction::SetActor( Actor* Obj )
{
    //TODO: generally this one will move to Action, In the end no actor will be set. Actor is a Component
    //Components have components. Will be checked by InitDependencies, and DependentComponents will be taken automatically
    //if the component fails to fulfil DependentComponents, it wont trigger the action.
    Action::SetActor( Obj );
}

void MoveAction::Update( double Seconds )
{
    if( !mActor )
    {
        return;
    }
    Action::Update( Seconds );
    Opt<IPositionComponent> actorPositionC = mActor->Get<IPositionComponent>();
    actorPositionC->SetX( actorPositionC->GetX() + Seconds * mActor->Get<IMoveComponent>()->GetSpeedX() );
    actorPositionC->SetY( actorPositionC->GetY() + Seconds * mActor->Get<IMoveComponent>()->GetSpeedY() );

    //TODO: this completly sucks. This should not be here, clipscene is one thing that i don't even like.
    if (mActor->Get<ICollisionComponent>().IsValid())
    {
        mActor->Get<ICollisionComponent>()->ClipScene();
    }
}