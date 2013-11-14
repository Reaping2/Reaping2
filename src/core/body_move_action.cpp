#include "i_core.h"

BodyMoveAction::BodyMoveAction(int32_t Id, Actor& actor)
	: Action(Id,actor)
{
	mAreBlockedActionsExcluded=false;
	mCancelledActionIds.push_back(IdStorage::Get().GetId("body_idle"));
	mSecsToEnd=1;
	mIsLoop=true;
}

void BodyMoveAction::Update(double Seconds) 
{
	Action::Update(Seconds);
}