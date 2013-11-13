#include "i_core.h"

void Actor::Update(double Seconds)
{
	if(mController.get())
		mController->Update(Seconds);
	// Controller->Update might change the actions!
	for(ActionDescList_t::iterator i=mActions.begin(),e=mActions.end();i!=e;++i)
		(*i)->Update(*this,Seconds);
}

void Actor::Collide( double Seconds, ActorList& Actors )
{
	for(ActorList::iterator i=Actors.begin(),e=Actors.end();i!=e;++i)
	{
		if(this==&*i) continue;	// don't self-collide
	}
}

Actor::Actor(std::string const& Name)
	:AutoId(Name)
{
	memset(&mFields,0,NUM_FIELDS*sizeof(Field_t));
	mFields[COLLISION_CLASS].i=CollisionClass::Player;
	mFields[TYPE_ID].i=mId;
}

void Actor::SetController( std::auto_ptr<Controller> Control )
{
	mController=Control;
	if(mController.get())
		mController->SetActor(this);
}
Action const* Actor::GetWeapon() const
{
	for(Actor::ActionDescList_t::const_iterator i=mActions.begin(),e=mActions.end();i!=e;++i)
		if ((*i)->GetType()==Action::Weapon)
			return *i;
	return NULL;
}
Action* Actor::GetActionDesc( int32_t Id )
{
	for(ActionDescList_t::iterator i=mActions.begin(),e=mActions.end();i!=e;++i)
		if((*i)->GetId()==Id)
			return *i;
	return NULL;
}

void Actor::AddAction( Action * Act )
{
	mActions.push_back(Act);
}

void Actor::DropAction( Action * Act )
{
	const int32_t ActionId=Act->GetId();
	for(ActionDescList_t::iterator i=mActions.begin(),e=mActions.end(),n;i!=e;i=n)
	{
		n=i;++n;
		if((*i)->GetId()==ActionId)
		{
			delete(*i);
			mActions.erase(i);
		}
	}
}
