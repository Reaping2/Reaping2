#include "i_core.h"

void Actor::Update(double Seconds)
{
	if(mController.get())
		mController->Update(Seconds);
	//mFields[X].d+=Seconds*mFields[SPEED_X].d;
	//mFields[Y].d+=Seconds*mFields[SPEED_Y].d;
	//mFields[ACTION_STATE].d+=100*Seconds*(1+rand()%4);
	//if(mFields[ACTION_STATE].d>=99)
	//	mFields[ACTION_STATE].d=0;
	ActionHolder::Get().Update(*this, Seconds);
}

void Actor::Collide( double Seconds, ActorList& Actors )
{
	for(ActorList::iterator i=Actors.begin(),e=Actors.end();i!=e;++i)
	{
		if(this==&*i) continue;	// don't self-collide
	}
}

Actor::Actor()
{
	memset(&mFields,0,NUM_FIELDS*sizeof(field_t));
	memset(&mFields,0,ACTION_COUNT*sizeof(double));
	mFields[COLLISION_CLASS].i=CollisionClass::Player;
}

void Actor::SetController( std::auto_ptr<Controller> Control )
{
	mController=Control;
	if(mController.get())
		mController->SetActor(this);
}

void Actor::SetActionIdPos(ActionHolder::ActionType ActionId, int32_t Position, bool Activate/*=true*/)
{
	if(ActionId<ActionHolder::MOVE||ActionId>=ActionHolder::NUM_FIELDS)return;
	if(Position<0||Position>3)return;
	if (Activate)
	{
		mFields[ACTION_ID].i|=ActionId<<(Position*8);
	}
	else
	{
		mFields[ACTION_ID].i&=~(0xFF<<(Position*8));
	}
}
void Actor::SetActionStatePos(int32_t Position, int32_t State)
{
	if(Position<0||Position>=ACTION_COUNT)return;
	mFields[ACTION_STATE].i&=State==0xFF?~(State<<(Position*8)):State;
}
bool Actor::HasAction(ActionHolder::ActionType ActionId, int32_t& Position)
{
	Position=-1;
	if(ActionId<ActionHolder::MOVE||ActionId>=ActionHolder::NUM_FIELDS)return false;
	int32_t gap=-1;
	int32_t action = mFields[ACTION_ID].i;
	for(size_t i=0;i<ACTION_COUNT;++i)
	{
		if (gap==-1&&(action&0xFF)==0)
		{
			gap=i;
		}
		if ((action&0xFF)==ActionId)
		{
			Position=i;
			return true;
		}
		action>>=8;
	}
	Position=gap;
	return false;
}

void Actor::ClearActions()
{
	this->SetAction(0);
	this->SetActionState(0);
	memset(&mActionStatePrecise,0,ACTION_COUNT*sizeof(double));
}

