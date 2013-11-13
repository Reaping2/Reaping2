#include"i_core.h"

ActionRepo::ActionRepo()
{	// ittene majd szepen beolvasva adatfajlbol a parameterezeseket (idotartam, sebesseg, sebzes, spawned creature name, fenetudja)
	mDefaultElement=boost::bind(&ActionRepo::Create<DefaultAction>);

	std::auto_ptr<Action> Act(new MoveAction()); int32_t Id=Act->GetId();
	mElements[Id]=boost::bind(&ActionRepo::Create<MoveAction>);
	Act.reset(new ShootAction()); Id=Act->GetId();
	mElements[Id]=boost::bind(&ActionRepo::Create<ShootAction>);
	Act.reset(new ShootAltAction()); Id=Act->GetId();
	mElements[Id]=boost::bind(&ActionRepo::Create<ShootAltAction>);
	Act.reset(new BodyMoveAction()); Id=Act->GetId();
	mElements[Id]=boost::bind(&ActionRepo::Create<BodyMoveAction>);
	Act.reset(new BodyIdleAction()); Id=Act->GetId();
	mElements[Id]=boost::bind(&ActionRepo::Create<BodyIdleAction>);
	Act.reset(new IdleAction()); Id=Act->GetId();
	mElements[Id]=boost::bind(&ActionRepo::Create<IdleAction>);
	Act.reset(new PlasmaGunAction()); Id=Act->GetId();
	mElements[Id]=boost::bind(&ActionRepo::Create<PlasmaGunAction>);
}
Action * ActionRepo::operator()( int32_t Id )
{
	return ((const ActionRepo*)this)->operator ()(Id);
}


Action * ActionRepo::operator()( int32_t Id ) const
{
	ElementMap_t::const_iterator i=mElements.find(Id);
	return (i==mElements.end()?mDefaultElement():(i->second)());
}

void ActionRepo::Register(int32_t Id, ActionFunctor functor)
{
	mElements[Id]=functor;
}
// ez mennyire gyilkosmar? :D
template<typename Element_T>
Element_T * ActionRepo::Create()
{
	return new Element_T();
}