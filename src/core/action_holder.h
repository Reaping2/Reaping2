#ifndef INCLUDED_ACTION_HOLDER_H
#define INCLUDED_ACTION_HOLDER_H

class Actor;
class Action;
class ActionHolder : public Singleton<ActionHolder>
{
	friend class Singleton<ActionHolder>;
	ActionHolder();
	~ActionHolder(){}
public:
	enum ActionType
	{
		MOVE=0,
		SHOOT,
		NUM_FIELDS
	};
	void AddAction(Actor& Who, ActionType What);
private:
	Action * Actions[NUM_FIELDS];


};

#endif//INCLUDED_ACTION_HOLDER_H