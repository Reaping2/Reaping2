#include "i_core.h"

BodyMoveAction::BodyMoveAction()
: Action("body_move")
{
	mAreBlockedActionsExcluded=false;
	mCancelledActionIds.push_back(IdStorage::Get().GetId("body_idle"));
	mSecsToEnd=1;
	mIsLoop=true;
}

void BodyMoveAction::Update(Actor& Actor, double Seconds) const
{
	Action::Update(Actor,Seconds);
}