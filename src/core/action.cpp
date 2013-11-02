#include "i_core.h"
const int32_t Action::ALL_ACTION=0xFFFFFF;
Action::Action()
	:mIsRefresh(false)
	,mBlocksActions(0)
	,mCancelsActions(0)
{
}

void Action::Activate(Actor& Actor, ActionHolder::ActionType What)
{
	bool blocked=false;
	int32_t actions = Actor.GetActionId();
	for(size_t i=0;i<4;++i)
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
		int32_t cancelActions=mCancelsActions>>1;
		for(int i=1;i<ActionHolder::NUM_FIELDS;++i)
		{
			if(cancelActions&1==1)
			{
				this->Deactivate(Actor,(ActionHolder::ActionType)i);
			}
			cancelActions>>=1;
		}
		int32_t pos;
		bool hasAction = Actor.HasAction(What, pos);
		if (!mIsRefresh&&hasAction)return;
		Actor.SetActionId(What,pos,true);
	}
}

void Action::Deactivate(Actor& Actor, ActionHolder::ActionType What)
{
	int32_t pos;
	bool hasAction = Actor.HasAction(What, pos);
	if (hasAction)
	{
		Actor.SetActionId(What,pos,false);
	}
}

bool Action::Blocks(ActionHolder::ActionType What) const
{
	return (mBlocksActions&(1<<What))!=0;
}
