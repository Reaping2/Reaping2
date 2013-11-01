#include "i_core.h"


void Actor::Update(double Seconds)
{
	if(mController.get())
		mController->Update(Seconds);
	mFields[X].d+=Seconds*mFields[SPEED_X].d;
	mFields[Y].d+=Seconds*mFields[SPEED_Y].d;
	mFields[ACTION_STATE].d+=100*Seconds*(1+rand()%4);
	if(mFields[ACTION_STATE].d>=99)
		mFields[ACTION_STATE].d=0;
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
	mFields[COLLISION_CLASS].i=CollisionClass::Player;
}

void Actor::SetController( std::auto_ptr<Controller> Control )
{
	mController=Control;
	if(mController.get())
		mController->SetActor(this);
}

