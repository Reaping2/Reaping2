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
		MOVE=1,
		SHOOT,
		NUM_FIELDS
	};
	void AddAction(Actor& Who, ActionType What);
	void RemoveAction(Actor& Who, ActionType What);
	const Action * GetAction(int32_t What) const;
private:
	Action * Actions[NUM_FIELDS];


};

#endif//INCLUDED_ACTION_HOLDER_H