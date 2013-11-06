#ifndef INCLUDED_ACTION_H
#define INCLUDED_ACTION_H

class Action: public AutoId
{
public:
	virtual ~Action(){}
	virtual void Update(Actor& Actor,double Seconds) const;
	virtual bool Activate(Actor& Actor) const;
	virtual void Deactivate(Actor& Actor) const;
protected:
	bool mIsRefresh;							// setting this action again, if its active, will it reset counter
	bool mIsLoop;								// reaching state 100 counter will continue from 0, otherwise it will remain at 100
	bool mIsSelfDestruct;						// reaching state 100 will automatically deactivate current action
	typedef std::vector<int32_t> ActionIdVec_t;
	bool mAreBlockedActionsExcluded;
	ActionIdVec_t mBlockedActionIds;			// list of actions this one blocks from activation
	bool mAreCancelledActionsExcluded;
	ActionIdVec_t mCancelledActionIds;			// list of actions this one cancels on activation
	double mSecsToEnd;							// speed. This much seconds is needed, till this action counts from 0..100.

	friend class ActionHolder;
	Action(std::string const& Name);

	bool Blocks(int32_t What) const;
	bool Cancels(int32_t What) const;
};

#endif//INCLUDED_ACTION_H
