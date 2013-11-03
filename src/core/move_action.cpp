#include "i_core.h"

MoveAction::MoveAction()
	:Action()
{
	mBlocksActions=ALL_ACTION;
	mCancelsActions|=1<<ActionHolder::SHOOT;
	mSecsToEnd=3;
	mIsLoop=true;
}

void MoveAction::Update(Actor& Actor, double Seconds) const
{
	Action::Update(Actor,Seconds);
	Actor.SetX(Actor.GetX()+Seconds*Actor.GetSpeedX());
	Actor.SetY(Actor.GetY()+Seconds*Actor.GetSpeedY());
}