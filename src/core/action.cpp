#include "i_core.h"

DefaultAction::DefaultAction(int32_t Id)
	: Action(Id)
{
	mIsRefresh=false;
	mIsLoop=false;
	mIsSelfDestruct=true;
	mAreBlockedActionsExcluded=false;
	mAreCancelledActionsExcluded=false;
	mSecsToEnd=0;
	mType=Normal;
}

void Action::SetActor(Actor* Obj)
{
	mActor=Obj;
}
bool DefaultAction::Activate() 
{
	return true;
}

void DefaultAction::Deactivate() 
{
}

void DefaultAction::Update(double Seconds) 
{
}


Action::Action(int32_t Id)
	: mId(Id)
	, mIsRefresh(false)
	, mIsLoop(false)
	, mIsSelfDestruct(false)
	, mAreBlockedActionsExcluded(false)
	, mAreCancelledActionsExcluded(false)
	, mSecsToEnd(1)
	, mState(0)
{
	LOG("!Action: id: %d\n",mId);
}

bool Action::Activate() 
{
	if(!mActor)return true;
	Actor::ActionList_t const& Actions=mActor->GetActions();
	for(Actor::ActionList_t::const_iterator i=Actions.begin(),e=Actions.end();i!=e;++i)
	{
		Action const& action=*i;
		if(action.Blocks(mId))
			return false;
	}
	//if this action cancels others
	for(Actor::ActionList_t::const_iterator i=Actions.begin(),e=Actions.end();i!=e;)
	{
		Action const& action=*i;
		++i;
		if(Cancels(action.GetId()))
		{
			mActor->DropAction(action.GetId());
		}
	}

	bool hasAction=false;
	Actor::ActionList_t::const_iterator i=Actions.begin(),e=Actions.end();
	while(!hasAction&&i!=e)
		hasAction=mId==(i++)->GetId();
	if (!mIsRefresh&&hasAction)
		return false;

	return true;
}

void Action::Deactivate() 
{
}

bool Action::Blocks(int32_t What) const
{
	return mAreBlockedActionsExcluded ^ (std::find(mBlockedActionIds.begin(),mBlockedActionIds.end(),What)!=mBlockedActionIds.end());
}

bool Action::Cancels(int32_t What) const
{
	return mAreCancelledActionsExcluded ^ (std::find(mCancelledActionIds.begin(),mCancelledActionIds.end(),What)!=mCancelledActionIds.end());
}

void Action::Update(double Seconds) 
{
	if(!mActor)return;
	Action& State=mActor->GetAction(mId);

	double nextState = State.GetState()+1./mSecsToEnd*Seconds*100.;
	if(nextState>=100)
	{
		if(mIsLoop)
			nextState=fmod(nextState,100.);
		else if(mIsSelfDestruct)
		{
			mActor->DropAction(mId);
			return;
		}
		else
			nextState=100.;
	}
	State.SetState(nextState);
	//LOG("nextState: %f %d\n",nextState,(int32_t)nextState);
}
