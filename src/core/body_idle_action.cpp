#include "i_core.h"

BodyIdleAction::BodyIdleAction()
: Action("body_idle")
{
	mAreBlockedActionsExcluded=false;
	mCancelledActionIds.push_back(IdStorage::Get().GetId("body_move"));
	mSecsToEnd=1;
	mIsLoop=true;
}

void BodyIdleAction::Update(Actor& Actor, double Seconds) 
{
	Action::Update(Actor,Seconds);
}