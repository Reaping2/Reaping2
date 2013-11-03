#include "i_core.h"
const int32_t Action::ALL_ACTION=0xFFFFFFF;
Action::Action()
	:mIsRefresh(false)
	,mIsLoop(false)
	,mIsSelfDestruct(false)
	,mBlocksActions(0)
	,mCancelsActions(0)
	,mSecsToEnd(1)
{
}

void Action::Activate(Actor& Actor, ActionHolder::ActionType What) const
{
	//if this action is blocked by others
	bool blocked=false;
	int32_t actions = Actor.GetActionId();
	for(size_t i=0;i<Actor::ACTION_COUNT;++i)
	{
		int32_t action=(actions&0xFF);
		if(action!=0)
		{
			const Action * realAction = ActionHolder::Get().GetAction(action);
			if (realAction)
			{
				if (realAction->Blocks(What))
				{
					blocked=true;
					break;
				}
			}
		}
		actions>>=8;
	}
	if (!blocked)
	{
		//if this action cancels others
		if (mCancelsActions!=0)
		{
			if (mCancelsActions==Action::ALL_ACTION)
			{
				Actor.ClearActions();
			}
			else
			{
				int32_t cancelActions=mCancelsActions>>1;
				for(int i=1;i<ActionHolder::NUM_FIELDS;++i)
				{
					if(cancelActions&1==1)
					{
						this->Deactivate(Actor,(ActionHolder::ActionType)i);
					}
					cancelActions>>=1;
				}
			}
		}
		int32_t pos;
		bool hasAction = Actor.HasAction(What, pos);
		if (!mIsRefresh&&hasAction)return;
		Actor.SetActionIdPos(What,pos,true);
		Actor.SetActionStatePos(pos);
		Actor.SetActionStatePrecise(pos,0.0);
	}
}

void Action::Deactivate(Actor& Actor, ActionHolder::ActionType What) const
{
	int32_t pos;
	bool hasAction = Actor.HasAction(What, pos);
	if (hasAction)
	{
		Actor.SetActionIdPos(What,pos,false);
		Actor.SetActionStatePos(pos);
		Actor.SetActionStatePrecise(pos,0.0);
	}
}

bool Action::Blocks(ActionHolder::ActionType What) const
{
	return (mBlocksActions&(1<<What))!=0;
}

void Action::SetState(Actor& Actor,double Seconds,int Position,ActionHolder::ActionType What) const
{
	double statePrecise=Actor.GetActionStatePrecise(Position);
	if (statePrecise<0||statePrecise>100)return;
	double nextState = statePrecise+1/mSecsToEnd*Seconds*100;
	if(nextState>100.0)
	{
		if (mIsLoop)
		{
			while(nextState>=100)
				nextState-=100.0;
		}
		else
		{
			nextState=100.0;
		}
	}
	if (nextState==100&&mIsSelfDestruct)
	{
		Deactivate(Actor,What);
	}
	Actor.SetActionStatePrecise(Position, nextState);
	Actor.SetActionStatePos(Position);
	Actor.SetActionStatePos(Position,(((int32_t)nextState)<<(Position*8)));
	LOG("nextState: %f %d\n",nextState,(int32_t)nextState);
}
