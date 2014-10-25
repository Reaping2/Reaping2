#ifndef INCLUDED_CORE_ACTION_H
#define INCLUDED_CORE_ACTION_H

class Action
{
public:
	virtual ~Action();
	virtual void SetActor(Actor* Obj);
	virtual void Update(double Seconds);
	virtual bool Activate();
	virtual void Deactivate();
protected:
	int32_t mId;
	Actor* mActor;
	bool mIsRefresh;							// setting this action again, if its active, will it reset counter
	bool mIsLoop;								// reaching state 100 counter will continue from 0, otherwise it will remain at 100
	bool mIsSelfDestruct;						// reaching state 100 will automatically deactivate current action
	typedef std::vector<int32_t> ActionIdVec_t;
	bool mAreBlockedActionsExcluded;
	ActionIdVec_t mBlockedActionIds;			// list of actions this one blocks from activation
	bool mAreCancelledActionsExcluded;
	ActionIdVec_t mCancelledActionIds;			// list of actions this one cancels on activation
	double mSecsToEnd;							// speed. This much seconds is needed, till this action counts from 0..100.
	double mState;

	friend class Factory<Action>;
	Action(int32_t Id);
public:
	bool Blocks(int32_t What) const;
	bool Cancels(int32_t What) const;
	double GetState()const{return mState;}
	void SetState(double S){mState=S;}
	int32_t GetId() const;
};

class DefaultAction : public Action
{
public:
	DefaultAction(int32_t Id);
	virtual void Update(double Seconds) ;
	virtual bool Activate() ;
	virtual void Deactivate() ;
};
#endif//INCLUDED_CORE_ACTION_H
