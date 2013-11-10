#include "i_core.h"

void Actor::Update(double Seconds)
{
	if(mController.get())
		mController->Update(Seconds);
	// Controller->Update might change the actions!
	for(ActionDescList_t::iterator i=mActions.begin(),e=mActions.end();i!=e;++i)
		i->GetAction()->Update(*this,Seconds);
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
Actor::ActionDesc_t const* Actor::GetWeapon() const
{
	for(Actor::ActionDescList_t::const_iterator i=mActions.begin(),e=mActions.end();i!=e;++i)
		if (i->GetAction()->GetType()==Action::Weapon)
			return &*i;
	return NULL;
}
Actor::ActionDesc_t* Actor::GetActionDesc( int32_t Id )
{
	for(ActionDescList_t::iterator i=mActions.begin(),e=mActions.end();i!=e;++i)
		if(i->GetId()==Id)
			return &*i;
	return NULL;
}

void Actor::AddAction( Action const& Act )
{
	mActions.push_back(ActionDesc_t(&Act));
}

void Actor::DropAction( Action const& Act )
{
	const int32_t ActionId=Act.GetId();
	for(ActionDescList_t::iterator i=mActions.begin(),e=mActions.end(),n;i!=e;i=n)
	{
		n=i;++n;
		if(i->GetId()==ActionId)
			mActions.erase(i);
	}
}

Actor::ActionDesc_t::ActionDesc_t( Action const* A,double S/*=0.*/ )
: mAction(A)
{
	mId.i=A->GetId();
	mState.d=S;
}
