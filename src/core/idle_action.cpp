#include "i_core.h"

IdleAction::IdleAction()
: Action("idle")
{
	mAreBlockedActionsExcluded=false;
	mCancelledActionIds.push_back(IdStorage::Get().GetId("move"));
	mSecsToEnd=1;
	mIsLoop=true;
}

void IdleAction::Update(Actor& Actor, double Seconds) 
{
	Action::Update(Actor,Seconds);
}