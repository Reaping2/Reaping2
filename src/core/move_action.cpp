#include "i_core.h"

MoveAction::MoveAction()
	: Action("move")
{
	mAreBlockedActionsExcluded=false;
	mCancelledActionIds.push_back(IdStorage::Get().GetId("idle"));
	mSecsToEnd=1;
	mIsLoop=true;
}

void MoveAction::Update(Actor& Actor, double Seconds) 
{
	Action::Update(Actor,Seconds);
	Actor.SetX(Actor.GetX()+Seconds*Actor.GetSpeedX());
	Actor.SetY(Actor.GetY()+Seconds*Actor.GetSpeedY());
	Actor.ClipScene();
}