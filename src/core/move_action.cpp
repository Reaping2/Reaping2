#include "i_core.h"

MoveAction::MoveAction(int32_t Id, Actor& actor)
	: Action(Id,actor)
{
	mAreBlockedActionsExcluded=false;
	mCancelledActionIds.push_back(IdStorage::Get().GetId("idle"));
	mSecsToEnd=1;
	mIsLoop=true;
}

void MoveAction::Update(double Seconds) 
{
	Action::Update(Seconds);
	mActor.SetX(mActor.GetX()+Seconds*mActor.GetSpeedX());
	mActor.SetY(mActor.GetY()+Seconds*mActor.GetSpeedY());
	mActor.ClipScene();
}