#include"i_core.h"
ActionHolder::ActionHolder()
{
	Actions[MOVE] = new MoveAction();
	Actions[SHOOT] = new ShootAction();
}
void ActionHolder::AddAction(Actor& Actor, ActionType What)
{
	if(What<MOVE||What>=NUM_FIELDS)return;
	Actions[What]->Activate(Actor,What);
}
void ActionHolder::RemoveAction(Actor& Actor, ActionType What)
{
	if(What<MOVE||What>=NUM_FIELDS)return;
	Actions[What]->Deactivate(Actor,What);
}
const Action * ActionHolder::GetAction(int32_t What) const
{
	if(What<MOVE||What>=NUM_FIELDS)return NULL;
	return Actions[What];
}
void ActionHolder::Update(Actor& Who, double Seconds)
{
	int32_t action = Who.GetActionId();
	int32_t state = Who.GetActionState();
	for(size_t i=0;i<Actor::ACTION_COUNT;++i)
	{
		const int32_t currAction = action&0xFF;
		const Action * realAction=GetAction(currAction);
		if(realAction)
		{
			realAction->SetState(Who,Seconds,i,(ActionType)currAction);
			int whereIsIt;
			if (Who.HasAction((ActionType)currAction,whereIsIt)&&whereIsIt==i)
			{
				realAction->Update(Who,Seconds);
			}
		}
			
		action>>=8;
	}
}
