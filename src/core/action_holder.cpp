#include"i_core.h"
ActionHolder::ActionHolder()
{
	Actions[MOVE] = new MoveAction();
	Actions[SHOOT] = new ShootAction();
}
void ActionHolder::AddAction(Actor& Who, ActionType What)
{
	Who.SetActionId(What);
}