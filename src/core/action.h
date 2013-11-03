#ifndef INCLUDED_ACTION_H
#define INCLUDED_ACTION_H

class Action
{
public:
	virtual ~Action(){}
protected:
	static const int32_t ALL_ACTION; // flag for all actions lül :D

	bool mIsRefresh; // setting this action again, if its active, will itt reset counter
	bool mIsLoop; // reaching state 100 counter will continue from 0, otherwise it will remain at 100
	bool mIsSelfDestruct; // reaching state 100 will automatically dactivate current action
	int mBlocksActions; // list of actions, this one blocks from actiovation
	int mCancelsActions; // list of actions, this one cancels on actiovation
	double mSecsToEnd; // speed. This much seconds is needed, till this action counts from 0..100.
	Action();
	virtual void Update(Actor& Actor,double Seconds) const{}
	virtual void Activate(Actor& Actor, ActionHolder::ActionType What) const;
	virtual void Deactivate(Actor& Actor, ActionHolder::ActionType What) const;
	bool Blocks(ActionHolder::ActionType What) const;
	virtual void SetState(Actor& Actor,double Seconds,int Position,ActionHolder::ActionType What) const;
	friend class ActionHolder;
};

#endif//INCLUDED_ACTION_H