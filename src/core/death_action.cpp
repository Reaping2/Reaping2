#include "i_core.h"

DeathAction::DeathAction(int32_t Id)
:Action(Id)
{
	mAreBlockedActionsExcluded=true;
	mAreCancelledActionsExcluded=true;
	mIsSelfDestruct=false;
	mSecsToEnd=0.7;
}

void DeathAction::Update( double Seconds )
{
	Action::Update(Seconds);
	if(!mActor)return;
	mActor->SetSpeed(0);
}
