#include "i_core.h"//

void Actor::Update(double Seconds)
{
	if(mController.get())
		mController->Update(Seconds);
	// Controller->Update might change the actions!
	for(ActionList_t::iterator i=mActions.begin(),e=mActions.end();i!=e;++i)
		i->Update(Seconds);
	for(ItemList_t::iterator i=mItems.begin(),e=mItems.end();i!=e;++i)
		i->Update(Seconds);
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
	,mActionFactory(Factory<Action>::Get())
	,mItemFactory(Factory<Item>::Get())
{
	memset(&mFields,0,NUM_FIELDS*sizeof(Field_t));
	mFields[COLLISION_CLASS].i=CollisionClass::Player;
	mFields[TYPE_ID].i=mId;
	mFields[RADIUS].d=3.0;
	mDefaultAction=AddAction(AutoId("default_action"));
	mDefaultItem=AddItem(AutoId("default_item"));
}

void Actor::SetController( std::auto_ptr<Controller> Control )
{
	mController=Control;
	if(mController.get())
		mController->SetActor(this);
}
Item& Actor::GetWeapon() 
{
	for(Actor::ItemList_t::iterator i=mItems.begin(),e=mItems.end();i!=e;++i)
		if (i->GetType()==Item::Weapon)
			return *i;
	return *mDefaultItem;
}

Item const& Actor::GetWeapon() const
{
	for(Actor::ItemList_t::const_iterator i=mItems.begin(),e=mItems.end();i!=e;++i)
		if (i->GetType()==Item::Weapon)
			return *i;
	return *mDefaultItem;
}

Action& Actor::GetAction( int32_t Id )
{
	for(ActionList_t::iterator i=mActions.begin(),e=mActions.end();i!=e;++i)
		if(i->GetId()==Id)
			return *i;
	return *mDefaultAction;
}

Action* Actor::AddAction( int32_t Id )
{
	Action * a=mActionFactory(Id);
	a->SetActor(this);
	if (a->Activate())	
	{
		mActions.push_back(a);
		return a;
	}
	return NULL;
}

Item& Actor::GetItem( int32_t Id )
{
	for(ItemList_t::iterator i=mItems.begin(),e=mItems.end();i!=e;++i)
		if(i->GetId()==Id)
			return *i;
	return *mDefaultItem;
}

Item* Actor::AddItem( int32_t Id )
{
	Item * a=mItemFactory(Id);
	a->SetActor(this);
	mItems.push_back(a);
	return a;
}

void Actor::DropAction( int32_t Id )
{
	for(ActionList_t::iterator i=mActions.begin(),e=mActions.end(),n;i!=e;i=n)
	{
		n=i;++n;
		if(i->GetId()==Id)
		{
			i->Deactivate();
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
	{
		delete this;
	}
}

Actor::~Actor()
{
	//for(ActionList_t::iterator i=mActions.begin(),e=mActions.end(),n;i!=e;i=n)
	//{
	//	n=i;++n;
	//	delete(*i);
	//	mActions.erase(i);
	//}
}
