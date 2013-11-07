#include "i_core.h"

DefaultAction::DefaultAction()
	: Action("default_action")
{
	mIsRefresh=false;
	mIsLoop=false;
	mIsSelfDestruct=true;
	mAreBlockedActionsExcluded=false;
	mAreCancelledActionsExcluded=false;
	mSecsToEnd=0;
}
bool DefaultAction::Activate(Actor& Actor) const
{
	return true;
}

void DefaultAction::Deactivate(Actor& Actor) const
{
}

void DefaultAction::Update( Actor& Object,double Seconds ) const
{
}


Action::Action(std::string const& Name)
	: AutoId(Name)
	, mIsRefresh(false)
	, mIsLoop(false)
	, mIsSelfDestruct(false)
	, mAreBlockedActionsExcluded(false)
	, mAreCancelledActionsExcluded(false)
	, mSecsToEnd(1)
{
	LOG("!Action: %s id: %d\n",Name.c_str(),mId);
}

bool Action::Activate(Actor& Actor) const
{
	Actor::ActionDescList_t const& Actions=Actor.GetActions();
	for(Actor::ActionDescList_t::const_iterator i=Actions.begin(),e=Actions.end();i!=e;++i)
	{
		Actor::ActionDesc_t const& action=*i;
		if(action.GetAction()->Blocks(mId))
			return false;
	}
	//if this action cancels others
	for(Actor::ActionDescList_t::const_iterator i=Actions.begin(),e=Actions.end();i!=e;)
	{
		Actor::ActionDesc_t const& action=*i;
		++i;
		if(Cancels(action.GetId()))
		{
			action.GetAction()->Deactivate(Actor);
		}
	}

	bool hasAction=false;
	Actor::ActionDescList_t::const_iterator i=Actions.begin(),e=Actions.end();
	while(!hasAction&&i!=e)
		hasAction=mId==(i++)->GetId();
	if (!mIsRefresh&&hasAction)
		return false;

	Actor.AddAction(*this);
	return true;
}

void Action::Deactivate(Actor& Actor) const
{
	Actor.DropAction(*this);
}

bool Action::Blocks(int32_t What) const
{
	return mAreBlockedActionsExcluded ^ (std::find(mBlockedActionIds.begin(),mBlockedActionIds.end(),What)!=mBlockedActionIds.end());
}

bool Action::Cancels(int32_t What) const
{
	return mAreCancelledActionsExcluded ^ (std::find(mCancelledActionIds.begin(),mCancelledActionIds.end(),What)!=mCancelledActionIds.end());
}

void Action::Update( Actor& Object,double Seconds ) const
{
	Actor::ActionDesc_t* State=Object.GetActionDesc(mId);
	if(!State)return;
	double nextState = State->GetState()+1./mSecsToEnd*Seconds*100.;
	if(nextState>=100)
	{
		if(mIsLoop)
			nextState=fmod(nextState,100.);
		else if(mIsSelfDestruct)
		{
			Deactivate(Object);
			return;
		}
		else
			nextState=100.;
	}
	State->SetState(nextState);
	//LOG("nextState: %f %d\n",nextState,(int32_t)nextState);
}
