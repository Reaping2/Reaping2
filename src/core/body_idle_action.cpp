#include "i_core.h"

BodyIdleAction::BodyIdleAction(int32_t Id, Actor& actor)
	: Action(Id,actor)
{
	mAreBlockedActionsExcluded=false;
	mCancelledActionIds.push_back(IdStorage::Get().GetId("body_move"));
	mSecsToEnd=1;
	mIsLoop=true;
}

void BodyIdleAction::Update(double Seconds) 
{
	Action::Update(Seconds);
}