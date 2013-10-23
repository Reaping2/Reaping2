#include "i_core.h"


void Actor::Update(double Seconds)
{
	SetHeading(Seconds*(rand()%10-5));
	SetSpeed(rand()%10-5);
	mFields[X].d+=Seconds*mFields[SPEED_X].d;
	mFields[Y].d+=Seconds*mFields[SPEED_Y].d;
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

