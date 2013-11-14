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
	,mActionRepo(ActionRepo::Get())
{
	memset(&mFields,0,NUM_FIELDS*sizeof(Field_t));
	mFields[COLLISION_CLASS].i=CollisionClass::Player;
	mFields[TYPE_ID].i=mId;
	mFields[RADIUS].d=3.0;
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

Action* Actor::AddAction( int32_t Id )
{
	Action * a=mActionRepo(Id);
	if (a->Activate(*this))	
	{
		mActions.push_back(a);
		return a;
	}
	return NULL;
}

void Actor::DropAction( int32_t Id )
{
	for(ActionDescList_t::iterator i=mActions.begin(),e=mActions.end(),n;i!=e;i=n)
	{
		n=i;++n;
		if((*i)->GetId()==Id)
		{
			(*i)->Deactivate(*this);
			delete(*i);
			mActions.erase(i);
		}
	}
}

void Actor::ClipScene()
{
	glm::vec4 AllowedDimensions=Scene::Get().GetDimensions();
	float Radius=(float)GetRadius();
	AllowedDimensions.x+=Radius;
	AllowedDimensions.y+=Radius;
	AllowedDimensions.z-=Radius;
	AllowedDimensions.w-=Radius;
	if(GetX()<AllowedDimensions.x)
		SetX(AllowedDimensions.x);
	else if(GetX()>AllowedDimensions.z)
		SetX(AllowedDimensions.z);
	if(GetY()<AllowedDimensions.y)
		SetY(AllowedDimensions.y);
	else if(GetY()>AllowedDimensions.w)
		SetY(AllowedDimensions.w);
}

void Actor::UpdateLifetime()
{
	if(GetHP()==HP_DEAD)
		delete this;
}

