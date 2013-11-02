#ifndef INCLUDED_ACTION_H
#define INCLUDED_ACTION_H

class Action
{
public:
	virtual ~Action(){}
protected:
	bool mIsRefresh; // setting this action again, if its active, will itt reset counter
	static const int32_t ALL_ACTION; // flag for all actions lül :D
	int mBlocksActions; // list of actions, this one blocks from actiovation
	int mCancelsActions; // list of actions, this one cancels on actiovation
	Action();
	virtual void Update(double Seconds, Actor& Actor){}
	virtual void Activate(Actor& Actor, ActionHolder::ActionType What);
	virtual void Deactivate(Actor& Actor, ActionHolder::ActionType What);
	bool Action::Blocks(ActionHolder::ActionType What) const;
	friend class ActionHolder;
};

#endif//INCLUDED_ACTION_H