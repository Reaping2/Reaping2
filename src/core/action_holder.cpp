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